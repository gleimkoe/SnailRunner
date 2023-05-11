///////////////////////////////////////////////////////////
//  Motor.cpp
//  Implementation of the Class Motor
//  Created on:      15-Jan-2010 21:03:50
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Motor.h"
#include "Controller.h"

Motor::Motor(){

}



Motor::~Motor(){

}


bool Motor::off(){
	bool retvalue=false;
	// --Check if is initialized.
	if (m_Controller) {
		retvalue=m_Controller->output(mport,LEFT,0);
		if (retvalue) {
			lvalue=0;
			ldirection=LEFT;
		}
	}
	return false;
}


bool Motor::connect(MotorPort port, Controller* controller){
	bool retvalue=false;
	// --Check if is not initialized.
	if (!m_Controller) {
		mport=port;
		retvalue=Controlled::connect(controller);	
		// --Configure Motor output.
		retvalue=controller->configure(port);
	}
	return retvalue;
}


bool Motor::on(Direction dir, int speed){
	bool retvalue=false;
	// --Check if is initialized.
	if (m_Controller) {
		retvalue=m_Controller->output(mport,dir,speed);
		if (retvalue) {
			lvalue=speed;
			ldirection=dir;
		}
	}
	return retvalue;
}

int Motor::last(){
	return lvalue;
}


Direction Motor::direction(){
	return ldirection;
}