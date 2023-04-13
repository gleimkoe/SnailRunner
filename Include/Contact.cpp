///////////////////////////////////////////////////////////
//  Contact.cpp
//  Implementation of the Class Contact
//  Created on:      09-Feb-2010 18:15:25
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Contact.h"

Contact::Contact(std::string text) :Sensor(DIGITAL_5KOMEGA,I1,text){

}



Contact::~Contact(){

}


bool Contact::isContacted(){
	// --Get the value and check if contacted
	int val=value();
	if (val!=-1 && val!=0)	
		return true;
	else
		return false;
}