#ifndef _runnerROBOT_H
#define _runnerROBOT_H

#define _USE_MATH_DEFINES

#include "ReactiveModel.h"
#include "Lamp.h"
#include "Button.h"
#include "Colour.h"
#include "Distance.h"
#include "IntelligentMotor.h"
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <vector>
#include <ctime>

/* --Include State Machines. */
#include "ExploreStateMachine.h"
#include "ObstacleStateMachine.h"
#include "ForwardStateMachine.h"
#include "SearchStateMachine.h"
#include "StartStateMachine.h"
#include "StartLauferMachine.h"
#include "WeiterLauferMachine.h"

#include "WaitUntil.h"

class SnailRunner : public ReactiveModel
{
public:

	/*! Der Snail Runner unterstützt 2 Missionen:
	 *  EXPLORE_MISSION   : Spurverfolgung
	 *  OBSTACLE_MISSION : Fährt auf Wand zu, stoppt und dreht; dreimalige Wiederholung. */
	enum Mission { EXPLORE_MISSION, OBSTACLE_MISSION, FORWARD_MISSION, SEARCH_MISSION, START_MISSION, START_LAUFER_MISSION, WEITER_LAUFER_MISSION };

	/*! Diese Methode setzen die nächste Mission. */
	void activate(Mission m) { mission=m; }

	/*! Der Standardkonstruktor. */
	SnailRunner();

	/*! Der Destruktor. */
	~SnailRunner(void);

	/*! Diese Methode konstruiert und initialisiert den kompletten Roboter. */
	bool construct(TxControllerSupervision* controller);

	/*! Mit diesen Methoden kann der Roboter gesteuert werden. */
	bool forward(double val, RunUnit unit =IMPULSE);
	bool backward(double val, RunUnit unit =IMPULSE);
	bool turnleft(double val, RunUnit unit =IMPULSE);
	bool turnright(double val, RunUnit unit =IMPULSE);
	bool stop();
	/* turn -Methode*/
	void turn(double grad);

	/*Grauwerte Queue*/
	bool detectGrey();

	/*! Diese Methoden liefern einzelne Komponenten des Roboters zurück. */
	IntelligentMotor& left() { return leftmotor; }
	IntelligentMotor& right() { return rightmotor; }
	Colour& colourback() { return colourSensorback; }
	Colour& colourdown() { return colourSensordown; }
	Distance& ahead() { return distance; }
	Distance& side() { return distance_side; }
	Lamp& lampfront() { return lamp_front; }
	Lamp& lampright() { return lamp_right; }
	Lamp& lampleft() { return lamp_left; }
	Lamp& lampcontroller() { return lamp_control; }
	Button& pushbutton() { return pushButton; }

	/*! Diese Methoden sind überladen und dienen zur Steuerung des Roboters
	 *  im reaktiven Modus (Steuerung über Zustandsmaschine. */
	void onCounterChanged(Bitfield bfield);
	void onMotorStopped(Bitfield bfield);
	void onInputChanged(Bitfield bfield);
	void onStart();
	void onStop();

	/* -- Diese Funktion vergleicht den ecke-Wert mit dem eingegebenen Wert -- */
	
	bool vergleich_ecke(int corner_amount);

	/* -- Funktion fuer StartLauferMachine -- */
	void sollecken_setzen(int vorgabe) { corner_amount = vorgabe; }
	void sollrunde_setzen(int vorgabe) { lap_amount = vorgabe; }
	void runde_hochzaehlen() { current_lap++; }

	/* -- Funktion fuer WeiterLauferMachine -- */
	

	/******************************************************************************/
	//
	//									KALIBRIERUNG
	//
	/******************************************************************************/
	// color values
	int white = 1100;
	int black = 1900;
	int grey = 1400;

	// threshold values Farbsensor unten
	int threshold_grey_low = 1479;//1500;
	int threshold_grey_high = 1870;//1600;
	int threshold_distance= 10;
	int threshold_distance_side_close = 5;
	int threshold_distance_side_far = 8;
	int threshold_distance_no_side = 20;

	vector<int> greyvalues;


	/******************************************************************************/
	//
	//									SETTINGS
	//
	/******************************************************************************/

	// true  - starter
	// false - follower
	bool start_position = true;

	//  1 - clockwise
	// -1 - counterclockwise
	int direction = 1;

	// amount of laps
	int lap_amount = 1;		// SollRunde
	int corner_amount = 0;  // SollEcken

	/******************************************************************************/
	//
	//									LOGGING
	//
	/******************************************************************************/
	std::clock_t start_time = 0;
	std::clock_t end_time = 0;

	int current_lap = 1;
	int current_corner = 0;
	int offtrail_count = 0;

	int obstacle_count = 0;

	double lapdistance = 0;

private:
	/*! Die Aktoren (Motoren und Lampen) des Roboters. */
	IntelligentMotor leftmotor;
	IntelligentMotor rightmotor;
	Lamp lamp_right;
	Lamp lamp_left;
	Lamp lamp_control;
	Lamp lamp_front;

	/*! Die Sensoren des Roboters. */
	Colour colourSensorback;
	Colour colourSensordown;		
	Distance distance;
	Distance distance_side;
	Sensor accuLevel;
	Button pushButton;

	//Werte zur Berechnung der Turn-Methode
	double rad_durchmesser = 0.05; //m
	int uebersetzungsverhaeltnis_getriebe = 20/10; // Motor-Gear/Wheel-Gear
	double runner_durchmesser = 0.16;
	double rad_umfang = rad_durchmesser * M_PI;
	double runner_umfang = runner_durchmesser * M_PI;
	double streckeRunnerimGrad = runner_umfang / 360;
	double radUmdrehungproGrad = streckeRunnerimGrad / rad_umfang;
	double grad_in_impulse = 75;
	
	/*! Geschwindigkeit der Roboters. */
	int speed;

	/*! Letzter Farbwert; wird benötigt um Erreichen von Schwellenwert zu bestimmen. */
	int last_colour_back;
	int last_colour_down;	
	int last_last_colour_down = 0;

	/*! Letzter Distanzwert; dito. */
	int last_dis;
	// Letzter Distanz rechter Sensor
	int last_dis_side; 

	/*! Zustandsmaschinen für die einzelnen Missionen. */
	ExploreStateMachine* ex_state;
	ObstacleStateMachine* ob_state;
	ForwardStateMachine* fw_state;
	SearchStateMachine* se_state;
	StartStateMachine* st_state;
	StartLauferMachine* sl_state;
	WeiterLauferMachine* wl_state;

	/*! Aktuelle Mission. */
	Mission mission;

};

#endif
