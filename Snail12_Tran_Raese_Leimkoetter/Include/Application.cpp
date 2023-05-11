///////////////////////////////////////////////////////////
//  Application.cpp
//  Implementation of the Class Application
//  Created on:      21-Mai-2010 11:53:20
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Application.h"


Application::Application(ControlLoop* l):function(l),timer(0){
	// --Create the timer object.
	timer=new Win32Timer;
	// --Connect the timer and the loop
	timer->trigger(function);
}

Application::~Application(){
	// --Delete the local timer.
	delete timer;
	// --Delete the other objects.
	delete function;
}

void Application::execute(int nMilliseconds){
	// --Check if is initialized.
	if (timer)
		timer->start(nMilliseconds);
}

void Application::terminate(){
	// --Check if is initialized.
	if (timer) {
		// --Cancel the timer.
		timer->cancel();
	}
}