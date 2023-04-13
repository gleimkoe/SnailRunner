///////////////////////////////////////////////////////////
//  Win32Timer.cpp
//  Implementation of the Class Win32Timer
//  Created on:      29-Jan-2010 22:28:21
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Win32Timer.h"
#include <process.h>

Win32Timer::Win32Timer():hTimer(NULL),tinterval(100),isPeriodic(true),isCanceled(true){
	hTimer = CreateWaitableTimer(NULL,FALSE,NULL);
	// --Create the termination flag.
	hTermination=CreateEvent(NULL,FALSE,FALSE,NULL);
}



Win32Timer::~Win32Timer(){
	CloseHandle(hTimer);
	CloseHandle(hTermination);
}

bool Win32Timer::start(int nMilliseconds, bool periodic){
	bool retvalue=false;
	// --Is initialized
	if (hTimer!=NULL) {
		// --Keep the parameter for re-start of the timer
		tinterval=nMilliseconds;
		isPeriodic=periodic;
		// --Create the Timer Thread Object.
		_beginthread(Win32Timer::threadProc,0,this);
	}
	return retvalue;
}


void Win32Timer::cancel(){
	SetEvent(hTermination);
	CancelWaitableTimer(hTimer);
	isCanceled=true;
}

void Win32Timer::timerProc(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue,
						   DWORD dwTimerHighValue) {
	// --The pointer is an object of Win32Timer. Static cast required...
	Win32Timer* wtimer=(Win32Timer*)lpArgToCompletionRoutine; 
	// --Now call the completion routine with the object.		
	wtimer->completion(dwTimerLowValue,dwTimerHighValue);
}

void Win32Timer::completion(DWORD /*dwTimerLowValue*/,DWORD /*dwTimerHighValue*/) {
	// --Call the alarm-routine and check the result.
	if (alarm()==false)
		cancel();
}

bool Win32Timer::restart() {
	bool retvalue=false;
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart=-10000*tinterval;
	// --Start the timer and wait
	if (SetWaitableTimer(hTimer,&liDueTime,isPeriodic?tinterval:0,(PTIMERAPCROUTINE)Win32Timer::timerProc,this,0)) {
		retvalue=true;
		isCanceled=false;
	}

	return retvalue;
}

void Win32Timer::threadProc(LPVOID pParam) {
	// --Cast the pointer to the timer object.
	Win32Timer* wtimer=(Win32Timer*)pParam;

	wtimer->restart();
	// --Wait for extern termination.
	DWORD ret=0;
	do 
		ret=WaitForSingleObjectEx(wtimer->hTermination,INFINITE,TRUE);
	while (ret==WAIT_IO_COMPLETION);
}



