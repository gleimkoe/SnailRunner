///////////////////////////////////////////////////////////
//  Counter.cpp
//  Implementation of the Class Counter
//  Created on:      25-Jan-2010 22:05:22
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Counter.h"
#include "Controller.h"


Counter::Counter(std::string text) :Sensor(COUNTER,C1,text){}

Counter::~Counter(){}


bool Counter::clear(){
	bool retvalue=false;
	// --Check if is initialized.
	if (m_Controller) 
		retvalue=m_Controller->clear(iport);
	return retvalue;
}
