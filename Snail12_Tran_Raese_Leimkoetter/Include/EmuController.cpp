///////////////////////////////////////////////////////////
//  EmuController.cpp
//  Implementation of the Class EmuController
//  Created on:      13-Jan-2010 22:32:51
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include <stdlib.h>

#include "EmuController.h"
#include "Direction.h"


// --Using some static array for simulation of counter values
static int countarray[4]={0,0,0,0};

EmuController::EmuController(){
	srand(1204);
	// --Clear the counter ports.
	clear(C1); clear(C2); clear(C3); clear(C4);
	// --Write output to console.
	cout << "EMULATION: initialization done." << endl;
}



EmuController::~EmuController(){
	// --Write output to console.
	cout << "EMULATION: finalization done." << endl;
}

bool EmuController::input(InputPort iPort, int& nValue, UniversalInput iType){
	bool retvalue=false;
	// --Check if initialized.
		retvalue=true;
		// --Select function based on input type.
		switch (iType) {
			case ANALOG_5KOMEGA :
				if (itype==RANDOM)
					nValue=rand()%512;
				else {
					cout << "GEBEN SIE FÜR PORT #" << iPort << " ANALOG [0-511] EIN: " << flush;
					cin >> nValue;
				}
				break;
			case DIGITAL_5KOMEGA : 
				if (itype==RANDOM)
					nValue=rand()%2;
				else {
					cout << "GEBEN SIE FÜR PORT #" << iPort << " DIGITAL [0|1] EIN: " << flush;
					cin >> nValue;
				}
				break;
			case ANALOG_10KV : 
				if (itype==RANDOM)
					nValue=rand()%512;
				else {
					cout << "GEBEN SIE FÜR PORT #" << iPort << " ANALOG [0-511] EIN: " << flush;
					cin >> nValue;
				}
				break;
			case DIGITAL_10KV : 
				if (itype==RANDOM)
					nValue=rand()%2;
				else {
					cout << "GEBEN SIE FÜR PORT #" << iPort << " DIGITAL [0|1] EIN: " << flush;
					cin >> nValue;
				}
				break;
			case DISTANCE : 
				if (itype==RANDOM)
					nValue=rand()%1024;
				else {
					cout << "GEBEN SIE FÜR PORT #" << iPort << " DISTANCE [0-1023] EIN: " << flush;
					cin >> nValue;
				}
				break;
			case COUNTER :
				// --Increment the counter array
				if (iPort>=C1 && iPort<=C4) {
					if (itype==RANDOM)
						countarray[iPort]+=rand()%2;
					else {
						cout << "GEBEN SIE FÜR PORT #" << iPort << " COUNTER [0..] EIN: " << flush;
						cin >> nValue;
						countarray[iPort]+=nValue;
					}
					nValue=countarray[iPort];
				} else
					retvalue=false;
				break;
			default :
				retvalue=false;
		};
		// --Write output to console.
		cout << "EMULATION: input port " << iPort << " value " << nValue << endl;
		retvalue=true;
	return retvalue;
}

bool EmuController::output(MotorPort oport, Direction dir, int speed){
	bool retvalue=false;
	const char* dirs[]={"Left","Right"};
		// --Write output to console.
		cout << "EMULATION: output port " << oport << " Direction " << (dir==LEFT? dirs[0] : dirs[1]) << " Speed " << speed << endl;
		retvalue=true;
	return retvalue;
}


bool EmuController::output(OutputPort oport, int value){
	bool retvalue=false;
		// --Write output to console.
		cout << "EMULATION: output port " << oport << " value " << value << endl;
		retvalue=true;
	return retvalue;
}

bool EmuController::clear(InputPort cPort) {
	bool retvalue=false;
	// --Check if correct port given.
	if (cPort>=C1 && cPort<=C4) {
		countarray[cPort]=0;
		retvalue=true;
	}
	return retvalue;
}


void EmuController::set(EmulationType et){
	itype=et;
}

bool EmuController::configure(InputPort /*iPort*/, UniversalInput /*iType*/) {
	return true;
}