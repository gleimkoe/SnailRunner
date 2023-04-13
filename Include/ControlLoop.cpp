///////////////////////////////////////////////////////////
//  ControlLoop.cpp
//  Implementation of the Class ControlLoop
//  Created on:      29-Jan-2010 22:24:23
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include <Windows.h>

#include "ControlLoop.h"

ControlLoop::ControlLoop():ncalls(0),nticks(0){

}



ControlLoop::~ControlLoop(){

}




EventField& ControlLoop::events(){
	return efield;
}

bool ControlLoop::step() {
	bool retvalue=false;
	// --If 1st call then init the start ticks
	if (ncalls==0)
		nticks=convertTicks();
	// --Increment the ncalls counter.
	ncalls++;
	// --First call the check method.
	if (check()) {
		retvalue=true;
		// --Loop until all events are cleared or something failed during the acting
		while (efield && retvalue)
			retvalue=act();
	}
	return retvalue;
}

unsigned int ControlLoop::calls(){
	return ncalls;
}


unsigned long long ControlLoop::ticks(){
	return convertTicks()-nticks;
}


unsigned long long ControlLoop::convertTicks(){
	// --Berechne die aktuelle Zeit und dann konvertieren
	SYSTEMTIME stime;
	FILETIME ftime;
	LARGE_INTEGER large;
	GetSystemTime(&stime);
	SystemTimeToFileTime(&stime,&ftime);
	large.LowPart=ftime.dwLowDateTime;
	large.HighPart=ftime.dwHighDateTime;
	return large.QuadPart;
}

void ControlLoop::bind(int ev,bool b0, bool b1, bool b2, bool b3, bool b4) {
	if (b0 && b1 && b2 && b3 && b4)
		efield.set(ev);
}
