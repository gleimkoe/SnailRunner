///////////////////////////////////////////////////////////
//  Controlled.cpp
//  Implementation of the Class Controlled
//  Created on:      09-Jan-2010 21:39:03
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Controlled.h"

Controlled::Controlled():m_Controller(0){}

Controlled::~Controlled(){}

bool Controlled::connect(Controller* control){
	bool retvalue=false;
	// --Check if not yet connected.
	if (!m_Controller) {
		m_Controller=control;
		retvalue=true;
	}
	return retvalue;
}

