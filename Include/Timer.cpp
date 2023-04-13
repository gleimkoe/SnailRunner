///////////////////////////////////////////////////////////
//  Timer.cpp
//  Implementation of the Class Timer
//  Created on:      17-Jan-2010 15:27:50
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Timer.h"

Timer::Timer():m_ControlLoop(0){
}



Timer::~Timer(){
}

void Timer::trigger(ControlLoop* callback){
	m_ControlLoop=callback;
}

bool Timer::alarm() {
	bool retvalue=false;
	if (m_ControlLoop) {
		// --Trigger the control loop
		retvalue=m_ControlLoop->step();
	}
	return retvalue;
}
