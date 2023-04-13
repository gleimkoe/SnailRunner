///////////////////////////////////////////////////////////
//  Actor.cpp
//  Implementation of the Class Actor
//  Created on:      09-Jan-2010 21:39:03
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Actor.h"
#include "Controller.h"

Actor::Actor(OutputPort po):oport(po),lvalue(-1){

}



Actor::~Actor(){

}



bool Actor::value(int val){
	bool retvalue=false;
	// --Check if initialized
	if (m_Controller) {
		retvalue=m_Controller->output(oport,val);
		if (retvalue)
			lvalue=val;
	}
	return retvalue;
}


bool Actor::connect(OutputPort port, Controller* controller){
	bool retvalue=false;
	// --Check if is not initialized.
	if (!m_Controller) {
		oport=port;
		retvalue=Controlled::connect(controller);	
		// --Configure PWM output.
		retvalue=controller->configure(port);
	}
	return retvalue;
}

void Actor::port(OutputPort po) {
	oport=po;
}

int Actor::last(){
	return lvalue;
}