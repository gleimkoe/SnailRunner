///////////////////////////////////////////////////////////
//  Button.cpp
//  Implementation of the Class Button
//  Created on:      25-Jan-2010 22:05:24
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Button.h"

Button::Button(std::string text) :Sensor(DIGITAL_5KOMEGA,I1,text){

}



Button::~Button(){

}


bool Button::isPressed(){
	// --Get the value and check if pressed
	int val=value();
	if (val!=-1 && val!=0)
		return true;
	else
		return false;
}

bool Button::isReleased() {
	// --Get the value and check if released
	if (value()==0)
		return true;
	else
		return false;
}