///////////////////////////////////////////////////////////
//  TrafficLight.cpp
//  Implementation of the Class TrafficLight
//  Created on:      09-Feb-2010 17:07:47
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "TrafficLight.h"

TrafficLight::TrafficLight(OutputPort pred,OutputPort pyellow,
						   OutputPort pgreen):
	redlight(pred),yellowlight(pyellow),greenlight(pgreen)
{

}



TrafficLight::~TrafficLight(){

}


void TrafficLight::redOn(){
	redlight.on();
}


void TrafficLight::redOff(){
	redlight.off();
}


void TrafficLight::yellowOn(){
	yellowlight.on();
}


void TrafficLight::yellowOff(){
	yellowlight.off();
}


void TrafficLight::greenOn(){
	greenlight.on();
}


void TrafficLight::greenOff(){
	greenlight.off();
}


void TrafficLight::allOn(){
	greenOn();
	redOn();
	yellowOn();
}


void TrafficLight::allOff(){
	greenOff();
	redOff();
	yellowOff();
}

void TrafficLight::port(OutputPort pred, OutputPort pyellow, 
						OutputPort pgreen) {
	// --Set the ports
	redlight.port(pred);
	yellowlight.port(pyellow);
	greenlight.port(pgreen);

}


bool TrafficLight::construct(Controller* controller){
	bool retvalue=Model::construct(controller);
	if (retvalue) {
		redlight.Controlled::connect(controller);
		yellowlight.Controlled::connect(controller);
		greenlight.Controlled::connect(controller);
	}
	return retvalue;
}


Lamp& TrafficLight::green(){
	return greenlight;
}


Lamp& TrafficLight::yellow(){
	return yellowlight;
}


Lamp& TrafficLight::red(){
	return redlight;
}

