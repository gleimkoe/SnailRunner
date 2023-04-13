#include <windows.h>
#include <process.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

#include "tx\common.h"
#include "tx\robo_tx_fw.h"
#include "tx\ftmsclib.h"
#include "tx\fterrcode.h"

#include "supervision.h"
#include "ReactiveModel.h"
#include "TxControllerSupervision.h"

//#define DEBUGME


const int SLEEPING_MS=20;	// --This is the sleep between 2 iterations of main loop.

// --Local copy of the transfer area.
static TA_ARRAY ta_copy; 

// --Is required to check changes at the input.
static TA_CHANGE ta_change;
static TA_INPUT ta_input;
static TA_OUTPUT ta_output;

// --Variable for motor reached signalisation (bitfield).
static ReactiveModel::Bitfield motorReached=0;

// --Annotation for output dump.
static bool isAnnotationAvailable = false;
static std::string annotation;

/* --Output stream. */
static std::ofstream fout;

// --Callback-Function.
void __stdcall OnMotorReached(DWORD /*devId*/, DWORD mtrIndx) {
	// --Set bit of motor index.
	motorReached|=(1<<mtrIndx);

#ifdef DEBUGME
	std::cout << "MOTOR REACHED ";
	for(int i=0;i<4;i++)
		std::cout << (motorReached & (1<<(3-i))?"0":"1");
	std::cout << std::endl;
#endif
}



// --Sync. variable for supervisior thread.
volatile bool done=false;

struct SupervisionArguments {
	const char* outfile;
	const char* infile;
	ReactiveModelMode mode;
	ReactiveModelPlayback playback;
	ReactiveModel* model;
} args;

// --Write sensor data with timestamp to file.
void DumpHeader() {
	const TxControllerLogging& logconf = args.model->controller()->logconf();
		// --Write configuration data 
	fout << "# GENERAL CONFIGURATION SETUP\n# DIGITAL INPUT (UNI x)\n";
	for (unsigned int i = 0; i < N_UNI; i++) {
		fout << "# " << std::setw(2) << i << ": ";
		if (!logconf.digital[i].isEnabled)
			fout << "-DISABLED";
		else
			fout << '+' << logconf.digital[i].name;
		fout << "\n";
	}
	fout << "# COUNTER (CNT x)\n";
	for (unsigned int i = 0; i < N_CNT; i++) {
		fout << "# " << std::setw(2) << i << ": ";
		if (!logconf.counter[i].isEnabled)
			fout << "-DISABLED";
		else
			fout << '+' << logconf.counter[i].name;
		fout << "\n";
	}
	// --Local time.
	fout << '#' << std::setw(5) << "CLCK|";
	// --Time stamp.
	fout << std::setw(7) << "TIME";
	// --Universal input
	for(int i=0;i<N_UNI;i++)
		if (logconf.digital[i].isEnabled)
			fout << "|UNI" << std::setw(2) << i;
	// --Counter values.
	for(int i=0;i<N_CNT;i++)
		if (logconf.counter[i].isEnabled)
			fout << "|CNT" << std::setw(2) << i;
	// --Motor values (output).
	for(int i=0;i<N_MOTOR;i++)
		if (logconf.counter[i].isEnabled)
			fout << "|DST" << std::setw(2) << i;
	fout << "|ANNOTATION" << std::endl;
}

void DumpSensor() {
	const TxControllerLogging& logconf = args.model->controller()->logconf();
	__int64 timestamp = args.model->current();
	// --Local time.
	time_t rawtime;
	time(&rawtime);
	struct tm* timeinfo;
	timeinfo=localtime(&rawtime);
	fout << std::setw(2) << timeinfo->tm_hour << ":" << std::setw(2) << timeinfo->tm_min;
	// --Time stamp.
	fout << std::setw(8) << timestamp;
	// --Universal input
	for(int i=0;i<N_UNI;i++)
		if (logconf.digital[i].isEnabled)
			fout << std::setw(6) << ta_copy.ftxTransferArea[TA_LOCAL].input.uni[i];
	// --Counter values.
	for(int i=0;i<N_CNT;i++)
		if (logconf.counter[i].isEnabled)
			fout << std::setw(6) << ta_copy.ftxTransferArea[TA_LOCAL].input.counter[i];
	// --Motor values (output).
	for (int i = 0; i < N_MOTOR; i++)
		if (logconf.counter[i].isEnabled)
			fout << std::setw(6) << ta_copy.ftxTransferArea[TA_LOCAL].output.distance[i];
	// --Annotation.
	if (isAnnotationAvailable)
		fout << " " << annotation;
	isAnnotationAvailable = false;
	fout << std::endl;
}

void SetAnnotationSupervision(const std::string& note) {
	/* --Check if annotation is already available. */
	if (isAnnotationAvailable)
		/* --Dump sensor+annotation data.*/
		DumpSensor();
	/* --Store annotation data. */
	annotation = note;
	isAnnotationAvailable = true;
}


// --Functions accessing the data from the transfer area copy.
int GetUniversalInput(InputPort port) {
	if (port>=I1 && port<=I8)
		return ta_copy.ftxTransferArea[TA_LOCAL].input.uni[port];
	else
		return -1;
}

int GetCounter(InputPort port) {
	if (port>=C1 && port<=C4)
		return ta_copy.ftxTransferArea[TA_LOCAL].input.counter[port];
	else
		return -1;
}

int GetCounterConfiguration(InputPort port) {
	if (port>=C1 && port<=C4)
		return ta_copy.ftxTransferArea[TA_LOCAL].input.cnt_in[port];
	else
		return -1;
}

void DoSupervision(void* arg) {
	SupervisionArguments* arguments=(SupervisionArguments*)arg; 
	ReactiveModel* model=arguments->model;
	TxControllerSupervision* controller=model->controller();
	HANDLE handle=controller->handle();

	// --Output file stream.
	fout.open(arguments->outfile);
	DumpHeader();
	// --Init time measurement.
	static __int64 t0, t1, frequency;
	QueryPerformanceCounter((LARGE_INTEGER*)&t0);
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	// --Set ratio to ms.
	frequency/=1000;

	// --Initial setting for the time.
	model->current(0);
	
	volatile TA_ARRAY* addr=GetTransferAreasArrayAddr(handle);

	// --Do an initial copy of the area.
	size_t sz_transfer=sizeof(TA_ARRAY);
	memcpy(&ta_copy,(void*)addr,sz_transfer);

	// --Copy for checking changes.
	size_t sz_change=sizeof(TA_CHANGE);
	size_t sz_input=sizeof(TA_INPUT);
	size_t sz_output=sizeof(TA_OUTPUT);
	memcpy(&ta_change,(void*)&addr->ftxTransferArea[TA_LOCAL].change,sz_change);
	memcpy(&ta_input,(void*)&addr->ftxTransferArea[TA_LOCAL].input,sz_input);
	memcpy(&ta_output,(void*)&addr->ftxTransferArea[TA_LOCAL].output,sz_output);

	// --Set the callback function.
	motorReached=0;
	SetCBMotorExReached(OnMotorReached);

	// --Start the robot.
	if (arguments->mode==REACTIVE) {
		model->setIsFinished(false);
		model->onStart();
	}
	
	ReactiveModel::Bitfield bfield;	
	// --Loop until done.	
	while(!done) {

		// --Timing information.
		QueryPerformanceCounter((LARGE_INTEGER*)&t1);

		// --Timer-Handling. 
		ReactiveModel::Bitfield timerevent=0;

		// --Setting current time.		
		model->current((t1-t0)/frequency);

		// --Loop on the timers. 		
		for(int i=0;i<NTIMER;i++) {
			TimerModel& timer=model->getTimer((TimerIdentifier)i);
			// --Timer active?
			if (timer.isActive) {
				// --Deadline reached?
				__int64 deadline=timer.alarm*frequency+t0;
				if (t1>=deadline) {
					timerevent|=(1<<i);
					// --Set new deadline or timer inactive.
					__int64 final_deadline=timer.final*frequency+t0;
					if (timer.final == TIMING_FOREVER || final_deadline>t1)
						timer.alarm+=timer.period;
					else
						timer.isActive=false;
				}
			}
		}
		// --Callback if some timer event.
		if (timerevent)
			model->onTimerChanged(timerevent);

		// --Wait until transferarea is ok.
//		while (ftxIsTransferActiv(handle)!=FTLIB_ERR_THREAD_IS_RUNNING);

		// --Make a local copy.
		memcpy(&ta_copy,(void*)addr,sz_transfer);
		ReactiveModel::Bitfield motorReached_copy=motorReached;

		// --First check if something has changed...
		if (memcmp(&ta_change,(void*)&ta_copy.ftxTransferArea[TA_LOCAL].change,sz_change) ||
			memcmp(&ta_input,(void*)&ta_copy.ftxTransferArea[TA_LOCAL].input,sz_input) ||
			memcmp(&ta_output,(void*)&ta_copy.ftxTransferArea[TA_LOCAL].output,sz_output)) {

				// --Check for changes at input channels.
				if (ta_copy.ftxTransferArea[TA_LOCAL].change.ChangeUni || 
					ta_copy.ftxTransferArea[TA_LOCAL].change.ChangeCntIn ||
					ta_copy.ftxTransferArea[TA_LOCAL].change.ChangeCounter ||
					motorReached_copy) {
							
						if (arguments->mode==REACTIVE) {

							// --Callback of the model.
							bfield=ta_copy.ftxTransferArea[TA_LOCAL].change.ChangeUni & 0xff;
							if (bfield & model->inputMask()) {
#ifdef DEBUGME
								std::cout << "INPUT CHANGED. MASK=" << (int)bfield << std::endl; 
#endif
								model->onInputChanged(bfield);
							}

							bfield=ta_copy.ftxTransferArea[TA_LOCAL].change.ChangeCounter & 0x0f;
							if (bfield & model->counterMask()) {
#ifdef DEBUGME
								std::cout << "COUNTER CHANGED. MASK=" << (int)bfield << std::endl; 
#endif
								model->onCounterChanged(bfield);
							}
							
							bfield=ta_copy.ftxTransferArea[TA_LOCAL].change.ChangeCntIn & 0x0f;
							if (bfield & model->counterConfMask()) {
#ifdef DEBUGME
								std::cout << "COUNTER CONFIGURATION CHANGED. MASK=" << (int)bfield << std::endl; 
#endif
								model->onCounterConfChanged(bfield);
							}
							// --Check the motors for stop.
							bfield=motorReached_copy;

							if (bfield & model->motorMask())
									model->onMotorStopped(bfield);
							
							// --Check the motors for abort.
							bfield=0;
							for(int i=0;i<N_MOTOR;i++) {
#if 0
								std::cout << ">" << (int) ta_copy.ftxTransferArea[TA_LOCAL].output.reserved[i] << ":"
									<< ta_copy.ftxTransferArea[TA_LOCAL].output.distance[i] << ":" << ta_output.distance[i] << "<" << std::endl;
#endif
#if 0								
								if (ta_copy.ftxTransferArea[TA_LOCAL].output.reserved[i]==0 &&
									ta_copy.ftxTransferArea[TA_LOCAL].output.distance[i]<ta_output.distance[i])				
										bfield|=(1<<i);
#endif
							}
							if (bfield & model->abortMask())
									model->onMotorAborted(bfield);
						} // --End reactive mode. 

				// --Write data to file.
				DumpSensor();
						
				// --Save for later comparison.
				memcpy(&ta_change,(void*)&ta_copy.ftxTransferArea[TA_LOCAL].change,sz_change);
				memcpy(&ta_input,(void*)&ta_copy.ftxTransferArea[TA_LOCAL].input,sz_input);
				memcpy(&ta_output,(void*)&ta_copy.ftxTransferArea[TA_LOCAL].output,sz_output);
				// --motorReached zurücksetzen.
				motorReached&=~motorReached_copy;
				} // --Changes at counter etc.

		} // --General changes.
		// --Wait another x ms.
		Sleep(SLEEPING_MS);
		// --Check if model should be stopped.
		if (args.mode==REACTIVE && model->isFinished()==true)
			done=true;
	}

	// --Stop the robot.
	if (arguments->mode==REACTIVE)
		model->onStop();
	
	// --Callback function zurücksetzen.
//	SetCBMotorExReached(NULL);
	
	// --Close the stream.
	fout.close();

}


void StartSupervision(ReactiveModel* model, ReactiveModelMode mode, const char* ofile, ReactiveModelPlayback pback, const char* /*ifile*/) {
	// --Assign the parameters.
	args.infile=ofile;
	args.mode=mode;
	args.model=model;
	args.outfile=ofile;
	args.playback=pback;
	done=false;
	_beginthread(DoSupervision,0,&args);
}

void StopSupervision() {
	done=true;
}
