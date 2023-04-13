///////////////////////////////////////////////////////////
//  Sensor.cpp
//  Implementation of the Class Sensor
//  Created on:      09-Jan-2010 21:39:04
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Sensor.h"
#include "Controller.h"

Sensor::Sensor(UniversalInput utype, InputPort ip, std::string text) :iport(ip), uType(utype),textual(text) {}


Sensor::~Sensor(){}


int Sensor::value(){
	int val=-1;
	// --Check if initialized.
	if (m_Controller)
		if (m_Controller->input(iport,val,uType)==false)
			val=-1;

	return val;
}


bool Sensor::connect(InputPort port, Controller* controller){
	bool retvalue=false;
	// --Check if is not initialized.
	if (!m_Controller) {
		iport=port;
		retvalue=Controlled::connect(controller);
		// --Configure the configure the controller.
		retvalue=controller->configure(port,uType,textual);
	}
	return retvalue;
}

void Sensor::port(InputPort ip) {
	iport=ip;
}

