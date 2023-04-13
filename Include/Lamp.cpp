///////////////////////////////////////////////////////////
//  Lamp.cpp
//  Implementation of the Class Lamp
//  Created on:      16-Jan-2010 14:48:41
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Lamp.h"

Lamp::Lamp(OutputPort po):Actor(po){

}



Lamp::~Lamp(){

}





bool Lamp::on(){
	return value(512);
}


bool Lamp::off(){
	return value(0);
}


bool Lamp::isOff(){
	return last() <= 0 ? true : false;
}


bool Lamp::isOn(){
	return last() > 0 ? true : false;
}


bool Lamp::toggle(){
	if (isOn())
		return off();
	else
		return on();
}