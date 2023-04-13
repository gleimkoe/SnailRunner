#include "TxControllerSupervision.h"
#include "supervision.h"

/* --Including the complete TXC stuff. */
#include "tx\common.h"
#include "tx\robo_tx_fw.h"
#include "tx\ftmsclib.h"
#include "tx\fterrcode.h"

// --Static object used for signalisation.
static HANDLE hCounterSignal=NULL;

// --Callback-Methode für Counter-Reset.
static void __stdcall CounterReset(DWORD,DWORD) {
	if (!SetEvent(hCounterSignal))	
		OutputDebugString(L"*** [FISHERMAN] COUNTER RESET SIGNALISATION FAILED ***\n");
}

TxControllerSupervision::TxControllerSupervision():_motorMode(EXECUTE_RUNNING_COMMAND),_logging() {
	// --Create the handle for signalisation.
	hCounterSignal=CreateEvent(NULL,FALSE,FALSE,TEXT("COUNTER SIGNAL"));
	// --Ok?
	if (hCounterSignal==NULL)
		OutputDebugString(L"*** [FISHERMAN] COUNTER SIGNALISATION CREATE FAILED ***\n");
	else
		SetCBCounterResetted(CounterReset);
}

TxControllerSupervision::~TxControllerSupervision() {
	CloseHandle(hCounterSignal);
}

bool TxControllerSupervision::configure(InputPort iPort, UniversalInput iType, std::string name) {
	bool retvalue=false;	
	// --Is the controller initialized?
	if (handle()!=NULL) {
		// --Check if counter
		if (iType==COUNTER) {
			// --Configure
			if (SetFtCntConfig(handle(), TA_LOCAL, iPort, 0) == FTLIB_ERR_SUCCESS) {
				// --Setup logging data.
				_logging.counter[iPort].type = COUNTER;
				_logging.counter[iPort].name = name;
				_logging.counter[iPort].isEnabled = true;
				retvalue = true;
			}
		} else {
			// --Not a counter
			bool digital=false; /* --Analog ANALOG_10KV. */
			int mode=0; /* --U */
			// --Check the input type
			if (iType==DIGITAL_10KV)
				digital=true;
			else if (iType==ANALOG_5KOMEGA)
				mode=1;
			else if (iType==DIGITAL_5KOMEGA) {
				mode=1;
				digital=1;
			} else if (iType==DISTANCE)
				mode=3;

			// --Configure the port.
			if (SetFtUniConfig(handle(), TA_LOCAL, iPort, mode, digital) == FTLIB_ERR_SUCCESS) {
				// --Setup logging data.
				_logging.digital[iPort].type = iType;
				_logging.digital[iPort].name = name;
				_logging.digital[iPort].isEnabled = true;
				retvalue = true;
			}
		}
	}
	return retvalue;
}



bool TxControllerSupervision::configure(MotorPort mport) {
	bool retvalue=false;	
	// --Is the controller initialized?
	if (handle()!=NULL) {
		// --Configure the motor port.
		if (SetFtMotorConfig(handle(),TA_LOCAL,mport,TRUE)==FTLIB_ERR_SUCCESS)
			retvalue=true;
	}
	return retvalue;
}



bool TxControllerSupervision::configure(OutputPort oport) {
	bool retvalue=false;
	// --Is the controller initialized?
	if (handle()!=NULL) {
		// --Configure as output port.
		if (SetFtMotorConfig(handle(),TA_LOCAL,oport/2,FALSE)==FTLIB_ERR_SUCCESS)
			retvalue=true;
	}
	return retvalue;
}

bool TxControllerSupervision::output(OutputPort oport, int value) {
	bool retvalue=false;
	// --Check if Controller is initialized.
	if (handle()!=NULL) {
		// --Set output value.
		if (SetOutPwmValues(handle(),TA_LOCAL,oport,value)==FTLIB_ERR_SUCCESS)
			retvalue=true;
	}
	return retvalue;
}

void TxControllerSupervision::annotate(const std::string& note) {
	SetAnnotationSupervision(note);
}

bool TxControllerSupervision::clear(InputPort cPort) {
	bool retvalue=false;
	// --Check if controller is initialized.
	if (handle()!=NULL) {
		// --Clear the counter
		if (StartCounterReset(handle(),TA_LOCAL,cPort)==FTLIB_ERR_SUCCESS) {			
			// --Wait until signalisation.
			if (WaitForSingleObject(hCounterSignal,INFINITE)==WAIT_OBJECT_0)
				retvalue=true;
			else
				OutputDebugString(L"*** [FISHERMAN] COUNTER SIGNALISATION WAIT FAILED ***\n");
		}
	}
	return retvalue;

}


bool TxControllerSupervision::input(InputPort iPort, int& nValue, UniversalInput iType) {
	bool retvalue=true;

	// --Is Counter?
	if (iType==COUNTER) {
		// --Get the value
		nValue=GetCounter(iPort);
	} else {
		// --Get the value
		nValue=GetUniversalInput(iPort);	
	}
	return retvalue;
}


bool TxControllerSupervision::output(MotorPort mport, Direction dir, int speed) {
	bool retvalue=true;
	// --Check if controller is initialized.
	if (handle()!=NULL) {
		int duty_p=0,duty_m=speed;
		if (dir==LEFT || dir==CLOCKWISE)
			duty_m=speed;
		else
			duty_p=speed;
		if (SetOutMotorValues(handle(),TA_LOCAL,mport,duty_p,duty_m)!=FTLIB_ERR_SUCCESS)
			retvalue=false;
	}
	return retvalue;
}

bool TxControllerSupervision::output(MotorPort mport, Direction dir, int speed, int impulse, MotorPort mport2, Direction dir2) {
	bool retvalue=true;
	// --Check if controller is initialized.
	if (handle()!=NULL) {
		if (StartMotorExCmd(handle(),TA_LOCAL,mport,speed,dir,mport2,dir2,impulse)!=FTLIB_ERR_SUCCESS)
			retvalue=false;
	}
	return retvalue;
}

bool TxControllerSupervision::stop(MotorPort mport) {
	bool retvalue=true;
	// --Check if controller is initialized.
	if (handle()!=NULL) {
		if (StopMotorExCmd(handle(),TA_LOCAL,mport)!=FTLIB_ERR_SUCCESS)
			retvalue=false;
	}
	return retvalue;
}

TxControllerSupervision::MotorMode TxControllerSupervision::motorMode() const { return _motorMode; }

void TxControllerSupervision::motorMode(MotorMode nmode) { _motorMode=nmode; }