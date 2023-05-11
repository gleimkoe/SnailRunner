///////////////////////////////////////////////////////////
//  Trail.cpp
//  Implementation of the Class Trail
//  Created on:      25-Jan-2010 22:05:25
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Trail.h"

Trail::Trail(std::string text) :Sensor(DIGITAL_10KV,I1,text), isTrackWhiteValue(false){

}



Trail::~Trail(){

}


bool Trail::isOnTrack(){
	if (isTrackWhiteValue)
		return value()==1 ? true : false;
	else
		return value()==0 ? true : false;
}


void Trail::trackWhite(){
	isTrackWhiteValue=true;
}


void Trail::trackBlack(){
	isTrackWhiteValue=false;
}


bool Trail::isTrackWhite(){
	return isTrackWhiteValue;
}