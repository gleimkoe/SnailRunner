#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <string>


using namespace std;

#include "Fisherman.h"
#include "WaitUntil.h"
#include "supervision.h"
#include "SnailRunner.h"
#include "TxControllerSupervision.h"
#include "LoggingServer.h"
#include "functions.h"

/* --Input Menue. */
void imenue(double& val, RunUnit& unit) {
	cout << "Distance to ride (impulses): " << flush;
	char buf[256], ch='i';
	cin >> buf;
	sscanf(buf,"%lf",&val);
	unit = IMPULSE; 
	cin.seekg(0, ios::end);
	cin.clear();
}

void testlamps(SnailRunner* robot) 
{
	cout << "Switch on Front... " << flush;
	robot->lampfront().on();
	WaitUntilIsOver(2000);
	cout << "Controller... " << flush;
	robot->lampcontroller().on();
	WaitUntilIsOver(2000);
	cout << "Left... " << flush;
	robot->lampleft().on();
	WaitUntilIsOver(2000);
	cout << "Right... " << flush;
	robot->lampright().on();
	WaitUntilIsOver(2000);
	cout << "Done." << endl;
	// --Some Disco lights
	cout << "Disco Lights... " << flush;
	for(int i=0;i<100;i++) {
		int c=rand()%4;
		switch (c) {
			case 0 : robot->lampfront().toggle(); break;
			case 1 : robot->lampcontroller().toggle(); break;
			case 2 : robot->lampleft().toggle(); break;
			case 3 : robot->lampright().toggle(); break;
		}
		WaitUntilIsOver(100);
	}
	// --Off.
	robot->lampfront().off();
	robot->lampcontroller().off();
	robot->lampleft().off();
	robot->lampright().off();

	cout << "Done." << endl;
	cout << "\nEnter 'OK' to continue" << endl;
	char dummy[10];
	cin>>dummy;
}

/* --TODO -- */
void WriteSensorDataToFile(SnailRunner* robot)
{
	int w; //Anzahl der zu ermittelnden Werte
	int time = 20; //Zeit = 20ms

	clock_t t;
	clock_t t_anfang; //Zeit vom Beginn

	int col_back_min = 0;
	int col_back_max = 0;
	int col_down_min = 0;
	int col_down_max = 0;
	int dis_min = 0;
	int dis_max = 0;
	int col_back_mittelwert = 0;
	double col_down_mittelwert = 0;
	double dis_mittelwert = 0;

	cout << "----- Datenlogger -----";
	cout << "Geben Sie die Anzahl der zu ermittelnde Werte ein (mind. 1, eher so 50): " ;
	cin >> w;

	if (w == 0)
	{
		cout << "Falsche Eingabe" << endl;
	}
	else
	{
		//Ausgabedatei erstellen und mit Ueberschriften fuellen
		ofstream ausgabe("Sensordaten.csv");
		ausgabe << "Zeit; Hinterer Farbsensor; Vorderer Farbsensor; Distanzsensor \n";

		t_anfang = clock();

		//Roboter abhaenging von der Anzahl der Messwerte vorwaerts fahren lassen
		robot->forward(4*w, IMPULSE);

		//Startwerte fuer Variablen
		col_back_min = robot->colourback().value();
		col_back_max = robot->colourback().value();
		col_down_min = robot->colourdown().value();
		col_down_max = robot->colourdown().value();
		dis_min = robot->ahead().value();
		dis_max = robot->ahead().value();

		for (int i = 1; i <= w; i++) //fuer jeden wert
		{
			//alle 20ms ...
			WaitUntilIsOver(time);

			//Timestamp ermitteln
			t = clock() - t_anfang;

			//Gebe gemessene Daten in Konsole aus
			cout << t << "; "  << robot->colourback().value() << "; " << robot->colourdown().value() << "; " << robot->ahead().value() << endl;

			//Schreibe gemessene Daten in .csv-Datei
			ausgabe << t << "; " << robot->colourback().value() << "; " << robot->colourdown().value() << "; " << robot->ahead().value() << endl;

			//geringster Wert hinterer Farbsensor wird gespeichert
			if (col_back_min > robot->colourback().value())
			{
				col_back_min = robot->colourback().value();
			}
			
			//hoechster Wert hinterer Farbsensor wird gespeichert
			if (col_back_max < robot->colourback().value())
			{
				col_back_max = robot->colourback().value();
			}

			//geringster Wert vorderer Farbsensor wird gespeichert
			if (col_down_min > robot->colourdown().value())
			{
				col_down_min = robot->colourdown().value();
			}

			//hoechster Wert vorderer Farbsensor wird gespeichert
			if (col_down_max < robot->colourdown().value())
			{
				col_down_max = robot->colourdown().value();
			}

			//geringster Wert Distanzsensor wird gespeichert
			if (dis_min > robot->ahead().value())
			{
				dis_min = robot->ahead().value();
			}

			//hoechster Wert Distanzsensor wird gespeichert
			if (dis_max < robot->ahead().value())
			{
				dis_max = robot->ahead().value();
			}

			dis_mittelwert += robot->ahead().value();
			col_down_mittelwert += robot->colourdown().value();
			col_back_mittelwert += robot->colourback().value();
		}

		//Mittelwertberechnung
		dis_mittelwert = dis_mittelwert / w;
		col_down_mittelwert = col_down_mittelwert / w;
		col_back_mittelwert = col_back_mittelwert / w;

		//schreibe min, max und Mittelwerte unter die Tabelle
		ausgabe << "\n\n";
		
		ausgabe << col_back_min << "; " << col_down_min << "; " << dis_min << "; " << "min" << endl;
		ausgabe << col_back_max << "; " << col_down_max << "; " << dis_max << "; " << "max" << endl;
		ausgabe << col_back_mittelwert << "; " << col_down_mittelwert << "; " << dis_mittelwert << "; " << "mittel" << endl;

		//gebe min, max und Mittelwerte in Konsole aus
		cout << col_back_min << "; " << col_down_min << "; " << dis_min << "; " << endl;
		cout << col_back_max << "; " << col_down_max << "; " << dis_max << "; " << endl;
		cout << col_back_mittelwert << "; " << col_down_mittelwert << "; " << dis_mittelwert << "; " << endl;

		//Datei wieder schliessen
		ausgabe.close();

		cout << "\nEnter 'OK' to continue" << endl;
		char dummy[10];
		cin >> dummy;
	}

}

void RotationControl(SnailRunner* robot /*runner*/) {
	cout << "todo..." << endl;
	double winkel = 0;
	cout << "Drehwinkel geben: ";
	cin >> winkel;
	robot->turn(winkel);
	cout << "\nEnter 'OK' to continue" << endl;
	char dummy[10];
	cin>>dummy;
}

/* --TODO -- */

/* --Main Menue. */
void menue() {
	bool done=false;

	/* --Ask for the configuration. */
	cout << "Hochschule Osnabrueck, Sommersemester 2023\n\n"
		 << "Software-Engineering fuer Elektrotechnik \n"
		 << "Programmiervorlage SNAIL RUNNER\n"
	     << endl << endl;
	
	cout << "Bluetooth/COM Configuration (0..): " << flush;
	int bt=0;
	cin >> bt;
		
	cout << "\nInitializing the system... " << flush;

	/* --Construct the Apha Robot */
	SnailRunner* runner=new SnailRunner;
	TxControllerSupervision* txsuper=new TxControllerSupervision;
	
	cout << "Version " << txsuper->major() << "." << txsuper->minor() << "." << txsuper->sub() << endl;

	/* --Open the input channel. */
	cout << "Open Port " << bt << (txsuper->open(bt) ? " OK" : "NOT OK") << endl;
	cout << "Configure & Initialize " << (runner->construct(txsuper)==true?"OK":"Failed") << endl;

	while (!done) {
		// --Clear the screen and wait for input by the user.
		system("cls");
		cout << "Hochschule Osnabrueck, Sommersemester 2023\n\n"
			<< "Software-Engineering fuer Elektrotechnik \n"
			<< "Programmiervorlage SNAIL RUNNER\n"
			<< endl;

		txsuper->display(); // Nur den Roboter-Namen TX-5xx ausgeben

		double val=0.0;
		RunUnit unit=IMPULSE;
		

		cout << "\n[W] Write Data To File (ToDo in Termin 2)\n"
			 << "[A] Turn Via Angle (ToDo in Termin 3)\n"
			 << "    --------------------------------\n"
			 << "[T] Test Lamps\n"
			 << "[F] Drive Forward\n"
			 << "[B] Drive Backward\n"
			 << "[L] Turn Left\n"
			 << "[R] Turn Right\n"
			 << "    --------------------------------\n"
			 << "State Machine Examples \n"
			 << "[1] State Machine Explore I (Termin 3)\n"    // class ExploreStateMachine (Spurverfolgung)
			 << "[2] State Machine Obstacle (Termine 7/8)\n"		// class ObstacleStateMachine (Hinderniserkennung)
			 << "[3] State Machine Explore III (Termine 7/8/9/10)\n"   // Aufteilung in zwei Statemachines unentbehrlich ab Termin 7
			                                                                      // Weitere Kandidaten: classes SearchStateMachine + ForwardStateMachine
			 << "[4] Start State Machine IIII (MS1)\n"
			 << "[5] Kalibrierung \n"
			 << "[6] Einstellungen\n"
			 << "[7] Start Laufer Machine \n"
			 << "[8] Weiter Laufer Machine \n"

			 << "    --------------------------------\n"
			 << "[Q] Quit"
			 << endl;

		char input='q';
		cin >> input;
		input=tolower(input);
		char dummy[256];

		// --Check user's wish.
		switch (input) {
		case 'q' : done=true;
			break;
		case 'w' : 
			StartSupervision(runner,NON_REACTIVE);
			WriteSensorDataToFile(runner);
			StopSupervision();
			break;
		case 'a' : 
			StartSupervision(runner,NON_REACTIVE);
			RotationControl(runner);
			StopSupervision();
			break;
		case 't' : 
			StartSupervision(runner,NON_REACTIVE);
			testlamps(runner);
			StopSupervision();
			break;
		case 'f' : // --Ask for further parameters.
			imenue(val,unit);
			StartSupervision(runner,NON_REACTIVE);
			runner->forward(val,unit);
			StopSupervision();
			break;
		case 'b' : // --Ask for more.
			imenue(val,unit);
			StartSupervision(runner,NON_REACTIVE);
			runner->backward(val,unit);
			StopSupervision();
			break;
		case 'l' : // --Ask for more.
			imenue(val,unit);
			StartSupervision(runner,NON_REACTIVE);
			runner->turnleft(val,unit);
			StopSupervision();
			break;
		case 'r' : //--Ask for more
			imenue(val,unit);
			StartSupervision(runner,NON_REACTIVE);
			runner->turnright(val,unit);
			//runner->turnright(75, IMPULSE);
			StopSupervision();
			break;
		case '1' :
			/* --Mission setzen. */
			runner->activate(SnailRunner::EXPLORE_MISSION);
			StartSupervision(runner);
			cout << "Explore I is running!" << endl
				 << "Note: a single statemachine will be used." << endl
				 << "Enter 'OK' to stop: " << flush;
			cin >> dummy;
			StopSupervision();
			break;
		case '2' :
			/* --Mission setzen. */
			runner->activate(SnailRunner::OBSTACLE_MISSION);
			StartSupervision(runner);
			cout << "Obstacle is running!! Enter 'OK' to stop: " << flush;
			cin >> dummy;
			StopSupervision();
			break;
		case '3':
			/* --Mission setzen. */
			runner->activate(SnailRunner::FORWARD_MISSION);
			StartSupervision(runner);
			cout << "Explore II is running!!" << endl
				<< "Note: two statemachines will be used." << endl
				<< "Enter 'OK' to stop: " << flush;
			cin >> dummy;
			StopSupervision();
			break;
		case '4':
			/* --Mission setzen. */
			int SollEcke;
			cout << "Anzahl der Ecken eingeben:";
			cin >> SollEcke; 
			runner->activate(SnailRunner::START_MISSION);
			StartSupervision(runner);
			runner->sollecken_setzen(SollEcke);
			
			cout << "Start State Machine is running!!" << endl
				<< "Enter 'OK' to stop: " << flush;
			cin >> dummy;
			StopSupervision();
			break;
		case '5':
			StartSupervision(runner, NON_REACTIVE);
			calibrationMenu(runner);
			StopSupervision();
			break;
		case '6':
			StartSupervision(runner, NON_REACTIVE);
			settingsMenu(runner);
			StopSupervision();
			break;
		case '7':
			int SollRunde;
			cout << "Anzahl der Runden eingeben:";
			cin >> SollRunde;
			runner->activate(SnailRunner::START_LAUFER_MISSION);
			StartSupervision(runner);
			runner->sollrunde_setzen(SollRunde);

			cout << "Start Laufer Machine is running!!" << endl
				<< "Enter 'OK' to stop: " << flush;
			cin >> dummy;
			StopSupervision();
			break;
		case '8':
			int SollRunde_1;
			cout << "Anzahl der Runden eingeben:";
			cin >> SollRunde_1;
			runner->activate(SnailRunner::WEITER_LAUFER_MISSION);
			StartSupervision(runner);
			runner->sollrunde_setzen(SollRunde_1);

			cout << "Weiter Laufer Machine is running!!" << endl
				 << "Enter 'OK' to stop: " << flush;
			cin >> dummy;
			StopSupervision();
			break;
		} ;
		cin.seekg(0, ios::end);
		cin.clear();
	}

	delete runner;
	delete txsuper;
}

// --Main Program.
int main() {
	// --Start the menue
	menue();
	return 0;
}