///////////////////////////////////////////////////////////
//  Win32Timer.h
//  Implementation of the Class Win32Timer
//  Created on:      21-Mai-2010 11:53:30
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_3EA134B4_5974_448d_BC88_3D718D2F396F__INCLUDED_)
#define EA_3EA134B4_5974_448d_BC88_3D718D2F396F__INCLUDED_

#include "Makros.h"
#include "Timer.h"

#include <windows.h>

class DLL_DECLSPEC Win32Timer : public Timer
{

public:
	Win32Timer();
	virtual ~Win32Timer();

	virtual bool start(int nMilliseconds, bool periodic = true);
	virtual void cancel();

protected:
	// --Internal method for handling timer events. 
	virtual void completion(DWORD dwTimerLowValue,DWORD dwTimerHighValue);
	
	// --(re-)start the windows' timer object.
	bool restart();

private:

	// --Internal method for handling windows' mechanism of timer objects.
	static void timerProc(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue,
							DWORD dwTimerHighValue);
	
	static void threadProc(LPVOID pParam);
		
	// --Timer object handle.
	HANDLE hTimer;
	HANDLE hTermination;

	// --Attributes.
	int tinterval;
	bool isPeriodic;
	bool isCanceled;
};
#endif // !defined(EA_3EA134B4_5974_448d_BC88_3D718D2F396F__INCLUDED_)
