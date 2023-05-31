#include "StartLauferMachine.h"
#include "SnailRunner.h"
#include "functions.h"

std::ofstream file("LogDatei.txt");

/* --Initialize State Description. */
const std::map<StartLauferMachine::State, std::string> StartLauferMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::START, "START" },
	{ State::SUCHEN, "SUCHEN" },
	{ State::AUSRICHTEN, "AUSRICHTEN" },
	{ State::AUSRICHTEN_2, "AUSRICHTEN_2" },
	{ State::AUSRICHTEN_3, "AUSRICHTEN_3" },
	{State::AUSRICHTEN_4, "AUSRICHTEN_4"},
	{ State::CORRECT_TRAIL_LEFT, "CORRECT_TRAIL_LEFT" },
	{ State::CORRECT_TRAIL_RIGHT, "CORRECT_TRAIL_RIGHT" },
	{ State::ON_TRAIL, "ON_TRAIL" },
	{ State::OFF_TRAIL, "OFF_TRAIL" },
	{ State::STOPPING, "STOPPING" },
	{ State::RELAY, "RELAY" },
	{ State::LAMPE_GRAY, "LAMPE_GRAY" },
	{ State::ENDE, "ENDE" },
	{ State::READY, "READY"},
	{ State::OBSTACLE_DETECTED, "OBSTACLE_DETECTED" },
	{ State::EVASION_1, "EVASION_1" },
	{ State::EVASION_2, "EVASION_2" },
	{ State::EVASION_3, "EVASION_3" },
	{ State::EVASION_4, "EVASION_4" },
	{ State::EVASION_5, "EVASION_5" },
	{ State::EVASION_6, "EVASION_6"},
	{ State::BACK_1, "BACK_1" },
	{ State::BACK_2, "BACK_2" },
	{ State::ALLIGN_1, "ALLIGN_1" },
	{ State::ALLIGN_2, "ALLIGN_2" },
	{State::VOR_FINAL, "VOR_FINAL"},
	{ State::FINAL, "FINAL" }
	/* --INFO: Here you should add new states for debugging purpose. */
};

const std::map<StartLauferMachine::Event, std::string> StartLauferMachine::EventDescription = {
		
	{ Event::WALL_AHEAD, "WALL_AHEAD" },
	{ Event::NOT_WALL_AHEAD, "NOT_WALL_AHEAD" },
	{ Event::OFF_TRAIL, "OFF_TRAIL" },
	{ Event::ON_TRAIL, "ON_TRAIL" },
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::ON_GREY, "ON_GREY" },
	{ Event::LICHT_HINTEN, "LICHT_HINTEN"},
	{ Event::NOT_LICHT_HINTEN, "NOT_LICHT_HINTEN"},
	{ Event::NO_SIDEWALL, "NO_SIDEWALL"}
	
	/* --INFO: Here you should add new events for debugging purpose. */
};

StartLauferMachine::StartLauferMachine(SnailRunner* r) :mystate(State::FAILURE), robot(r), count(1) {}

/* --INFO: Do not touch this method. */
void StartLauferMachine::state(State s) {
	/* --Log state chance. Check if description is available. */
	bool isOldStateAvailable = (StateDescription.find(mystate) != StateDescription.end());
	bool isNewStateAvailable = (StateDescription.find(s) != StateDescription.end());
	/* --If program stops here, you have to add a state description to the map at top of this file !!! */
	if (!isOldStateAvailable && !isNewStateAvailable)
		OutputDebugString((LPCTSTR)"*** CHECK YOUR CODE! STATE DESCRIPTION IS MISSING ***");

	/* --Log string. */
	std::string note = std::string("OLD STATE:") + (isOldStateAvailable ? StateDescription.at(mystate) : "*ERROR*")
		+ std::string(" --> NEW STATE:") + (isNewStateAvailable ? StateDescription.at(s) : "*ERROR*");
	robot->controller()->annotate(note);

	/* --Set new state.*/
	mystate = s;
}

/* --INFO: Do not touch this method. */
StartLauferMachine::State StartLauferMachine::state() const { return mystate; }

void StartLauferMachine::start() {
	/* --Log string. */
	robot->controller()->annotate("RE/START START STATE MACHINE");
	/* --Set everything to initial values. */
	count = 1;

	/* --Start with initial transition. */
	onEnteringStart();
	startTimer(robot);
	/* --INFO: Here you can change/add initial values when state machine is started. */

	robot->current_lap = 1;
	robot->current_corner = 0;
	robot->greyvalues.clear();

}

void StartLauferMachine::restart() {
	/* --Same as start. */
	start();
}

/* --INFO: Do not touch this method. */
void StartLauferMachine::handle(Event ev) {
	State old = mystate;

	/* --Logging information. */
	bool isEventAvailable = (EventDescription.find(ev) != EventDescription.end());
	/* --If program stops here, you have to add an event description to the map at top of this file !!! */
	if (!isEventAvailable)
		OutputDebugString((LPCTSTR)"*** CHECK YOUR CODE! EVENT DESCRIPTION IS MISSING ***");
	/* --Log string. */
	std::string note = std::string("EVENT TRIGGER:") + (isEventAvailable ? EventDescription.at(ev) : "*ERROR*");
	robot->controller()->annotate(note);

	/* --Process the event. */
	transition(ev);

	/* --Check if state was changed. Could be an issue with your state machine.*/
	if (old == mystate)
		robot->controller()->annotate(std::string("MAYBE UNHANDLED TRIGGER!"));
}

void StartLauferMachine::transition(Event ev) {
	/* --State machine as nested switch case. */
	switch (mystate) {
	case State::START:
		switch (ev) {
		case StartLauferMachine::Event::WALL_AHEAD:onLeavingStart(); onEnteringStart();
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:onLeavingStart(); onEnteringSuchen();
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingStart(); onEnteringFailure();
		}
		break;

	case State::SUCHEN:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:onLeavingSuchen(); onEnteringSuchen();
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY: onLeavingSuchen(); onEnteringAusrichten();
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingSuchen(); onEnteringFailure();
		}
		break;

	case State::AUSRICHTEN:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED: onLeavingAusrichten(); onEnteringAusrichten_2();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default:onLeavingAusrichten(); onEnteringFailure();
		}
		break;

	case State::AUSRICHTEN_2:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			if (robot->start_position == false)
			{
				onLeavingAusrichten_2(); onEnteringAusrichten_3();
			}
			break;
		case StartLauferMachine::Event::ON_GREY: 
			if (robot->start_position == true)
			{
				onLeavingAusrichten_2(); onEnteringAusrichten_3();
			}
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default:onLeavingAusrichten_2(); onEnteringFailure();
		}
		break;

	case State::AUSRICHTEN_3:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL: 
			if (robot ->start_position == true) // STARTLAUFER
			{
				onLeavingAusrichten_3(); onEnteringOnTrail(); logLapBanner(robot, file);
			}
			else
			{
				onLeavingAusrichten_3(); onEnteringEnde(); 
			}
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			/*if (robot->start_position == false)
			{
				onLeavingAusrichten_3(); onEnteringAusrichten_4(); 
			}*/
			break;
		case StartLauferMachine::Event::ON_GREY: onLeavingAusrichten_3(); onEnteringAusrichten_3();
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default:onLeavingAusrichten_3(); onEnteringFailure();
			
		}
		break;

	/*case State::AUSRICHTEN_4:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL: onLeavingAusrichten_4(); onEnteringEnde() ; logLapBanner(robot, file);
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingAusrichten_4(); onEnteringFailure();
			
		}
		break;
		*/
	case State::ON_TRAIL:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD: 
			if ( robot->current_corner < 4)
			{
				onLeavingOnTrail(); onEnteringObstacleDetected();
			}
		
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:onLeavingOnTrail(); onEnteringOffTrail();
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:onLeavingOnTrail(); onEnteringOnTrail();
			break;
		case StartLauferMachine::Event::ON_GREY:  
			 if (robot->current_corner >= 4) // && robot ->current_lap <= robot ->lap_amount)
			{
				cout << "ON_TRAIL ZU RELAY" << endl;
				onLeavingOnTrail(); onEnteringRelay();
			}
			/*
			else if (robot->current_corner >= 4 && robot->current_lap == robot->lap_amount)
			{
				
				cout << "ON_TRAIL ZU VOR_FINAL" << endl;
				onLeavingOnTrail(); onEnteringVorFinal();
				
			}
			*/
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingOnTrail(); onEnteringFailure();
		}
		break;

	case State::OFF_TRAIL:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED: onLeavingOffTrail(); onEnteringCorrectTrailLeft();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingOffTrail(); onEnteringFailure();
		}
		break;

	case State::STOPPING:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED: onLeavingStopping(); onEnteringOnTrail();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingStopping(); onEnteringFailure();
		}
		break;

	case State::CORRECT_TRAIL_LEFT:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:onLeavingCorrectTrailLeft(); onEnteringStopping();
			break;
		case StartLauferMachine::Event::IS_STOPPED: onLeavingCorrectTrailLeft(); onEnteringCorrectTrailRight();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingCorrectTrailLeft(); onEnteringFailure();
		}
		break;

	case State::CORRECT_TRAIL_RIGHT:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL: onLeavingCorrectTrailRight(); onEnteringStopping();
			break;
		case StartLauferMachine::Event::IS_STOPPED: onLeavingCorrectTrailRight(); onEnteringCorrectTrailLeft();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::RELAY:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			if (robot->start_position == false && robot ->current_lap == robot ->lap_amount)
			{
				getElapsedTime(robot);
				logLapConclusion(robot, file);
				cout << "RUNDE_CNT" << endl;
				onLeavingRelay(); onEnteringFinal();
			}
			else 
			{
				onLeavingRelay(); onEnteringLampeGray();
			}
			
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD: 
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default:onLeavingRelay(); onEnteringFailure();

		}
		break;

	case State::LAMPE_GRAY:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD: 
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD: 
			if (robot->current_corner >= 4 && robot->current_lap < robot->lap_amount)
			{
				cout << "LAMPE_GRAY ZU READY" << endl;
				onLeavingLampeGray(); onEnteringReady(); 
				logLapConclusion(robot, file);
				robot->current_lap = robot->current_lap + 1;
			}
			else if (robot->current_corner >= 4 && robot->current_lap == robot->lap_amount)
			{
				if (robot->start_position == true) // StartLaufer
				{
					cout << "LAMPE_GRAY ZU VOR_FINAL" << endl;
					onLeavingLampeGray(); onEnteringVorFinal();
				}
				/*else // WeiterLaufer
				{
					cout << "LAMPE_GRAY ZU FINAL" << endl;
					onLeavingLampeGray(); onEnteringFinal();
				}*/
				

			}
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::READY:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:onLeavingReady(); onEnteringReady();
			break;
		case StartLauferMachine::Event::ON_TRAIL:onLeavingReady(); onEnteringEnde();
			break;
		case StartLauferMachine::Event::IS_STOPPED: //onLeavingReady(); onEnteringEnde();
			break;
		case StartLauferMachine::Event::ON_GREY: onLeavingReady(); onEnteringReady();
			break;
		case StartLauferMachine::Event::LICHT_HINTEN: 
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::ENDE:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN: onLeavingEnde(); onEnteringOnTrail(); logLapBanner(robot, file); resetDistance(robot);
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default:onLeavingEnde(); onEnteringFailure();
		}
		break;

	/* ------ HINDERNIS UMFAHREN -------------*/

	case State::OBSTACLE_DETECTED:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingObstacleDetected(); onEnteringEvasion1();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingObstacleDetected(); onEnteringFailure();

		}
		break;

	case State::EVASION_1:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD: onLeavingEvasion1(); onEnteringEvasion2();
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingEvasion1(); onEnteringFailure();

		}
		break;

	case State::EVASION_2:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingEvasion2(); onEnteringEvasion3();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingEvasion2(); onEnteringFailure();

		}
		break;

	case State::EVASION_3:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingEvasion3(); onEnteringEvasion4();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingEvasion3(); onEnteringFailure();

		}
		break;

	case State::EVASION_4:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingEvasion4(); onEnteringEvasion5();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingEvasion4(); onEnteringFailure();

		}
		break;

	case State::EVASION_5:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL: onLeavingEvasion5(); onEnteringEvasion6();
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingEvasion5(); onEnteringFailure();

		}
		break;

	case State::EVASION_6:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingEvasion6(); onEnteringBack1();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingEvasion6(); onEnteringFailure();

		}
		break;


	case State::BACK_1:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingBack1(); onEnteringBack2();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingBack1(); onEnteringFailure();

		}
		break;

	case State::BACK_2:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL: onLeavingBack2(); onEnteringAllign1();
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingBack2(); onEnteringFailure();

		}
		break;

	case State::ALLIGN_1:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED: onLeavingAllign1(); onEnteringAllign2();
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingAllign1(); onEnteringFailure();

		}
		break;

	case State::ALLIGN_2:
		switch (ev)
		{
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::ON_TRAIL: onLeavingAllign2(); onEnteringOnTrail();
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onLeavingAllign2(); onEnteringFailure();

		}
		break;

	case State::VOR_FINAL:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL: onLeavingVorFinal(); onEnteringFinal(); getElapsedTime(robot); logLapConclusion(robot, file);
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingVorFinal(); onEnteringFailure();
			
		}
		break;

	case State::FINAL:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default:;
		}
		break;

	default: state(State::FAILURE);
	};
}

void StartLauferMachine::onEnteringFailure() {
	state(State::FAILURE);
	robot->stop();
	robot->lampleft().on();
}

void StartLauferMachine::onEnteringStart() { // wait
	state(State::START);
	cout << "Start" << endl;
	logStartConditions(robot, file);
}

void StartLauferMachine::onEnteringSuchen() { //forward 
	state(State::SUCHEN);
	robot->forward(1, METER);
	count = 1;
	cout << "Suchen" << endl;
}

void StartLauferMachine::onEnteringAusrichten() { // forward(10cm)
	state(State::AUSRICHTEN);
	robot->forward(0.1, METER);
	cout << "Ausrichten" << endl;
}

void StartLauferMachine::onEnteringAusrichten_2() { // turn(-180)
	state(State::AUSRICHTEN_2);
	if (robot->start_position == true) // STARTLAUFER
	{
		robot->turn(/*robot->direction */ 180); // -180 rechts drehen
	}
	else
	{
		robot->turn(70);
	}
	

	cout << "Ausrichten_2" << endl;
}

void StartLauferMachine::onEnteringAusrichten_3() {
	state(State::AUSRICHTEN_3);
	//if (robot->start_position == true) // STARTLAUFER
	{
		robot->forward(0.5, METER);
	}
	/*else
	{
		robot->turn(7.5);
	}*/
	
	robot->lampright().on();
	cout << "Ausrichten_3" << endl;
}

void StartLauferMachine::onEnteringAusrichten_4() {
	state(State::AUSRICHTEN_4);
	robot->forward(1, METER);
	cout << "Ausrichten_4" << endl;
}

void StartLauferMachine::onEnteringOnTrail() {// forward
	state(State::ON_TRAIL);
	robot->forward(1, METER);
	robot->lampright().on();
	count = 1;
	cout << "OnTrail" << endl;
}

void StartLauferMachine::onEnteringOffTrail() {// stop
	logOFF_TRAIL(robot, file);
	state(State::OFF_TRAIL);
	
	robot->stop();
	cout << "OffTrail" << endl;
}

void StartLauferMachine::onEnteringStopping() {// ecken_cnt()
	state(State::STOPPING);
	cout << "Stopping" << endl;
}

void StartLauferMachine::onEnteringCorrectTrailRight() { // turn()
	state(State::CORRECT_TRAIL_RIGHT);
	count *= 2;
	if (count == 16)
	{
		robot->current_corner = robot->current_corner + 1;
		logCorner(robot, file);
		/*if (robot->current_corner == 4)
		{
			robot->current_lap = robot->current_lap + 1;
			cout << "Lap Nr, : " << robot->current_lap << endl;
		}
		*/
		cout << "Ecke Nr. : " << robot->current_corner << endl;
		
	}
	robot->turn(count * -7.5);
	cout << "CorrectRight" << endl;
}

void StartLauferMachine::onEnteringCorrectTrailLeft() { // turn()
	state(State::CORRECT_TRAIL_LEFT);
	count *= 2;
	if (count == 16)
	{
		robot->current_corner = robot->current_corner + 1;
		logCorner(robot, file);
		/*
		if (robot->current_corner == 4)
		{
			robot->current_lap = robot->current_lap + 1;
			cout << "Lap Nr, : " << robot->current_lap << endl;
		}*/
		cout << "Ecke Nr. : " << robot->current_corner << endl;
	}

	robot->turn(count * 7.5);
	cout << "CorrectLeft" << endl;
}

void StartLauferMachine::onEnteringFinal() {
	state(State::FINAL);
	robot->stop();
	endTimer(robot);
	robot->lampright().off();
	robot->lampfront().off();
	robot->lampleft().on();
	cout << "Final" << endl;
}

void StartLauferMachine::onEnteringLampeGray() {
	state(State::LAMPE_GRAY);
	robot->stop();
	robot->lampfront().on();
	//robot ->current_lap = robot->current_lap + 1;
	
	cout << "LampeGray" << endl;
	
	cout << "Staffel Übergabe" << endl;
	
}

void StartLauferMachine::onEnteringReady() {
	state(State::READY);

	cout << "Runde Nr: " << robot-> current_lap << endl;
	//logLapConclusion(robot, file);
	robot ->current_corner = 0;
	robot->lampfront().off();
	robot->lampright().off();
	robot->forward(1, METER);	
	cout << "Ready" << endl;
}

void StartLauferMachine::onEnteringRelay() {
	state(State::RELAY);
	robot->forward(0.5,METER);
	//robot->stop();
	
	cout << "RELAY" << endl;
}

void StartLauferMachine::onEnteringEnde() {
	state(State::ENDE);
	robot->stop();
	
	cout << "Ende" << endl;
}

void StartLauferMachine::onEnteringVorFinal() {
	state(State::VOR_FINAL);
	robot->forward(1, METER);
	robot->lampfront().off();
	robot->lampright().off();
	cout << "VOR_FINAL" << endl;
}

void StartLauferMachine::onLeavingAusrichten() {} // Stop
	
void StartLauferMachine::onLeavingAusrichten_2() {
	robot->stop();
	cout << "AUSRICHTEN_2 STOP" << endl;
}

void StartLauferMachine::onLeavingAusrichten_3() {}

void StartLauferMachine::onLeavingAusrichten_4() {}

void StartLauferMachine::onLeavingOnTrail()
{
	addtoDistance(robot);
	//cout << robot->lapdistance << endl;//robot->left().encoder().value() << endl;
	cout << ((robot->lapdistance / 60.0) / 2.0)*M_PI*0.05/*((1.0 / 75.0)* robot->lapdistance*M_PI*0.05 / 360.0) / 100.0 */ << "m" << std::endl;
}

void StartLauferMachine::onLeavingStopping() {}

void StartLauferMachine::onLeavingOffTrail() {
	robot->forward(0.1, METER);
}
void StartLauferMachine::onLeavingFailure() {}


void StartLauferMachine::onLeavingCorrectTrailLeft() { // stop
	robot->stop();
}
void StartLauferMachine::onLeavingCorrectTrailRight() {// stop
	robot->stop();
}
void StartLauferMachine::onLeavingFinal() {}

void StartLauferMachine::onLeavingStart()
{
	if (robot->start_position == false)
	{
		WaitUntilIsOver(2000);
	}
}

void StartLauferMachine::onLeavingSuchen(){ // Stop
	robot->stop();
}

void StartLauferMachine::onLeavingLampeGray() {}

void StartLauferMachine::onLeavingRelay(){}

void StartLauferMachine::onLeavingEnde() {}

void StartLauferMachine::onLeavingReady() {}

void StartLauferMachine::onLeavingVorFinal() {}

/* --------- HINDERNIS UMFAHREN --------------*/

void StartLauferMachine::onEnteringObstacleDetected() {
	state(State::OBSTACLE_DETECTED);
	robot->stop();
	logObstacle(robot, file);
	cout << "OBSTACLE_DETECTED" << endl;
}

void StartLauferMachine::onEnteringEvasion1() {
	state(State::EVASION_1);
	robot->turn(90);
	cout << "EVASION_1" << endl;
}

void StartLauferMachine::onEnteringEvasion2() {
	state(State::EVASION_2);
	robot->turn(40); // muss noch mal anpassen!!!
	cout << "EVASION_2" << endl;

}

void StartLauferMachine::onEnteringEvasion3() {
	state(State::EVASION_3);
	robot->forward(0.3, METER);
	cout << "EVASION_3" << endl;

}

void StartLauferMachine::onEnteringEvasion4() {
	state(State::EVASION_4);
	robot->turn(-70);
	cout << "EVASION_4" << endl;

}

void StartLauferMachine::onEnteringEvasion5() {
	state(State::EVASION_5);
	robot->forward(1, METER);
	cout << "EVASION_5" << endl;

}

void StartLauferMachine::onEnteringEvasion6() {
	state(State::EVASION_6);
	robot->forward(0.05, METER);
	cout << "EVASION_6" << endl;

}

void StartLauferMachine::onEnteringBack1() {
	state(State::BACK_1);
	robot->turn(-70);
	cout << "BACK_1" << endl;

}

void StartLauferMachine::onEnteringBack2() {
	state(State::BACK_2);
	robot->forward(1, METER);
	cout << "BACK_2" << endl;

}

void StartLauferMachine::onEnteringAllign1() {
	state(State::ALLIGN_1);
	robot->forward(0.1, METER);
	cout << "ALLIGN_1" << endl;

}

void StartLauferMachine::onEnteringAllign2() {
	state(State::ALLIGN_2);
	robot->turn(180);
	cout << "ALLIGN_2" << endl;

}

void StartLauferMachine::onLeavingObstacleDetected() {}

void StartLauferMachine::onLeavingEvasion1() {
	robot->stop();
}

void StartLauferMachine::onLeavingEvasion2() {}

void StartLauferMachine::onLeavingEvasion3() {}

void StartLauferMachine::onLeavingEvasion4() {}

void StartLauferMachine::onLeavingEvasion5() {
	robot->stop();
}

void StartLauferMachine::onLeavingEvasion6() {}

void StartLauferMachine::onLeavingBack1() {}

void StartLauferMachine::onLeavingBack2() {}

void StartLauferMachine::onLeavingAllign1() {}

void StartLauferMachine::onLeavingAllign2() {}