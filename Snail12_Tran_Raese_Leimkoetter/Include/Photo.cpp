///////////////////////////////////////////////////////////
//  Photo.cpp
//  Implementation of the Class Photo
//  Created on:      16-Jan-2010 14:48:40
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "Photo.h"

Photo::Photo(std::string text) :Sensor(ANALOG_10KV,I1,text), darkvalue(0){

}



Photo::~Photo(){

}


bool Photo::isDark(){
	return value()<=darkvalue ? true : false;
}


int Photo::darkness(){
	return darkvalue;
}


void Photo::darkness(int ndark){
	darkvalue=ndark;
}