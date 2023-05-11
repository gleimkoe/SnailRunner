#define _USE_MATH_DEFINES
#include <cmath>

#include "IntelligentMotor.h"

IntelligentMotor::IntelligentMotor(std::string text) :Motor(), im_per_rot(75), wheel_dia(0.05), wheel_counter(text), syncMotor(0) {}

IntelligentMotor::~IntelligentMotor(){}


unsigned int IntelligentMotor::impulse(double val, RunUnit unit) const {
	unsigned int retvalue=0;
	// --Tranform the values to impulses.
	switch (unit) {
		case IMPULSE : retvalue=(unsigned int)val;
					   break;
		case ROTATION: retvalue=(unsigned int)(val*im_per_rot);
					   break;
		case DEG360  : retvalue=(unsigned int)(val/360.0*im_per_rot); 
                       break;
		case METER   : retvalue=(unsigned int)(val/(wheel_dia*M_PI)*im_per_rot);
					   break;
	}
	return retvalue;
}


bool IntelligentMotor::turnOn(double num, RunUnit unit, int speed, Direction dir, Direction dir2){
	bool retvalue=false;
	/* --Dynamic cast. */
	TxControllerSupervision* controller=dynamic_cast<TxControllerSupervision*>(m_Controller);
	// --Already initialized
	if (controller) {
		// --Calculate the number of impulse to drive.
		unsigned int imp=impulse(num,unit);
		// --Controller
		if (syncMotor)
			retvalue=controller->output(mport,dir,speed,imp,syncMotor->mport,dir2);
		else
			retvalue=controller->output(mport,dir,speed,imp,mport,dir);
	}
	// --Already initialized
	return retvalue;
}


bool IntelligentMotor::turnOff(){
	bool retvalue=false;
	/* --Dynamic cast. */
	TxControllerSupervision* controller=dynamic_cast<TxControllerSupervision*>(m_Controller);
	if (controller)
		retvalue=controller->stop(mport);
	/* --Notwendig oder nicht? */
	Sleep(20);
	return retvalue;
}


/**
 * Synchronize the object with a slave encoder motor.
 */
bool IntelligentMotor::synchronize(IntelligentMotor* slave){
	if (syncMotor!=slave) {
		syncMotor=slave;
		if (slave)
			slave->synchronize(this);
	}
	return true;
}


unsigned int IntelligentMotor::rotation() const {
	return im_per_rot;
}


void IntelligentMotor::rotation(unsigned int val){
	im_per_rot=val;
}


double IntelligentMotor::diameter() const {
	return wheel_dia;
}


void IntelligentMotor::diameter(double val){
	wheel_dia=val;
}



bool IntelligentMotor::connect(MotorPort po, InputPort co, TxControllerSupervision* con){
	bool retvalue=false;
	// --Call super class.
	if (Motor::connect(po,con)==true)
		// --Init the counter
		retvalue=wheel_counter.connect(co,con);
	return retvalue;
}

Counter& IntelligentMotor::encoder() {
	return wheel_counter;
}