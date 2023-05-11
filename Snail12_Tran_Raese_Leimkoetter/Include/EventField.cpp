///////////////////////////////////////////////////////////
//  EventField.cpp
//  Implementation of the Class EventField
//  Created on:      10-Feb-2010 14:37:33
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "EventField.h"

EventField::EventField():flags(0){
}



EventField::~EventField(){
}





bool EventField::set(int ev){
	bool retvalue=false;
	if (ev>=0 && ev<length()) {
		// --Set the bit at ev position to 1.
		flags|=(unsigned long long)1<<ev;
		retvalue=true;
	}		
	return retvalue;
}


bool EventField::clear(int ev){
	bool retvalue=false;
	if (ev>=0 && ev<length()) {
		// --Set the bit at ev position to 0.
		flags&=~(1<<ev);
		retvalue=true;
	}		
	return retvalue;
}


bool EventField::isSet(int ev){
	if (ev >= 0 && ev < length())
		// --Check if the bit at ev position is 1.
		return (flags & (1ull << ev)) ? true : false;
	else
		return false;
}


bool EventField::isSetAndClear(int ev){
	bool retvalue=isSet(ev);
	clear(ev);
	return retvalue;
}


EventField::operator bool(){
	return flags ? true : false;	
}


void EventField::reset(){
	bits(0);
}


void EventField::bits(unsigned long long pattern){
	flags=pattern;
}


unsigned long long EventField::bits(){
	return flags;
}


int EventField::length(){
	return sizeof(unsigned long long)*8;
}