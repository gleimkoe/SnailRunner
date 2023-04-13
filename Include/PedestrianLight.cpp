///////////////////////////////////////////////////////////
//  PedestrianLight.cpp
//  Implementation of the Class PedestrianLight
//  Created on:      09-Feb-2010 18:12:31
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "PedestrianLight.h"

PedestrianLight::PedestrianLight(OutputPort pred,OutputPort pgreen):
	redlight(pred),greenlight(pgreen)
{

}



PedestrianLight::~PedestrianLight(){

}





void PedestrianLight::redOn(){
	redlight.on();
}


void PedestrianLight::redOff(){
	redlight.off();
}


void PedestrianLight::greenOn(){
	greenlight.on();
}


void PedestrianLight::greenOff(){
	greenlight.off();
}

Button& PedestrianLight::button() {
	return pbutton;
}

void PedestrianLight::port(OutputPort pred,OutputPort pgreen) {
	redlight.port(pred);
	greenlight.port(pgreen);
}


bool PedestrianLight::construct(Controller* controller){
	bool retvalue=Model::construct(controller);
	if (retvalue) {
		redlight.Controlled::connect(controller);
		greenlight.Controlled::connect(controller);
		pbutton.Controlled::connect(controller);
	}
	return retvalue;
}


Lamp& PedestrianLight::green(){
	return greenlight;
}


Lamp& PedestrianLight::red(){
	return redlight;
}

