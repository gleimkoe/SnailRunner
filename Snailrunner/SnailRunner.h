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

/* --Include State Machines. */
#include "ExploreStateMachine.h"
#include "ObstacleStateMachine.h"
#include "ForwardStateMachine.h"
#include "SearchStateMachine.h"
#include "StartStateMachine.h"

#include "WaitUntil.h"

class SnailRunner : public ReactiveModel
{
public:

	/*! Der Snail Runner unterstützt 2 Missionen:
	 *  EXPLORE_MISSION   : Spurverfolgung
	 *  OBSTACLE_MISSION : Fährt auf Wand zu, stoppt und dreht; dreimalige Wiederholung. */
	enum Mission { EXPLORE_MISSION, OBSTACLE_MISSION, FORWARD_MISSION, SEARCH_MISSION, START_MISSION };

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
	/*! Diese Methoden liefern einzelne Komponenten des Roboters zurück. */
	IntelligentMotor& left() { return leftmotor; }
	IntelligentMotor& right() { return rightmotor; }
	Colour& colourback() { return colourSensorback; }
	Colour& colourdown() { return colourSensordown; }
	Distance& ahead() { return distance; }
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

	/* -- Diese Funktion vergleicht den ecke-Wert mit dem eingegebenen Wert*/
	
	bool vergleich(int SollEcken);
	void sollecken_setzen(int vorgabe) { SollEcken = vorgabe; }


private:

	int SollEcken;

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
	int speed;			/*! Geschwindigkeit der Roboters. */
	int last_colour_down;	/*! Letzter Farbwert; wird benötigt um Erreichen von Schwellenwert zu bestimmen. */
	int last_dis;		/*! Letzter Distanzwert; dito. */

	/*! Zustandsmaschinen für die einzelnen Missionen. */
	ExploreStateMachine* ex_state;
	ObstacleStateMachine* ob_state;
	ForwardStateMachine* fw_state;
	SearchStateMachine* se_state;
	StartStateMachine* st_state;

	/*! Aktuelle Mission. */
	Mission mission;

};

#endif
