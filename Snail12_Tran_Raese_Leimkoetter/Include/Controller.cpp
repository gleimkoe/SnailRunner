///////////////////////////////////////////////////////////
//  Controller.cpp
//  Implementation of the Class Controller
//  Created on:      09-Jan-2010 21:39:03
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

// --Fisherman's Best Friend interface.
#include "Controller.h"
#include "Controlled.h"
#include "fversion.h"


Controller::Controller():m_Controller(0){

}



Controller::~Controller(){

}

void Controller::banner(ostream& out) const {
	out << "FISHERMAN VERSION: " << fishermanVersion() << endl;
	out << "LIBRARY VERSION  : " << major() << '.' << minor() << '.' << sub() << endl;
	out << "FIRMWARE VERSION : " << firmware() << endl;
	out << "MANUFACTURER     : " << manufacturer() << endl;
	out << "SHORT NAME       : " << shortname() << endl;
	out << "LONG NAME        : " << name() << endl;
	out << "SERIAL NUMBER    : " << serialnumber() << endl;		
}