#ifndef WEITERLAUFERMACHINE_H
#define WEITERLAUFERMACHINE_H

#include <map>
#include <string>

#include "WaitUntil.h"

/* --Forward declaration. */
class SnailRunner;

class WeiterLauferMachine {
public:

	enum class State { FAILURE, START, SUCHEN, AUSRICHTEN, AUSRICHTEN_2, AUSRICHTEN_3, CORRECT_TRAIL_LEFT, CORRECT_TRAIL_RIGHT, ON_TRAIL, OFF_TRAIL, STOPPING, RELAY, LAMPE_GRAY, ENDE, READY, FINAL };
	enum class Event { WALL_AHEAD, NOT_WALL_AHEAD, OFF_TRAIL, ON_TRAIL, IS_STOPPED, ON_GREY, RUNDE_CNT, NOT_RUNDE_CNT, LICHT_HINTEN, NOT_LICHT_HINTEN };

	WeiterLauferMachine(SnailRunner* r);
	/* --Returns the current state. */
	State state() const;
	/* --Process the event using state transitions. */
	void handle(Event ev);

	/* --Start the state machine, set initial state. */
	void start();
	/* --Restart the state machine. */
	void restart();

private:
	State mystate;
	SnailRunner* robot;
	int count;

	/* -- Methods called when entering a state. */
	void onEnteringFailure();
	void onEnteringStart(); // wait
	void onEnteringSuchen(); //forward 
	void onEnteringAusrichten(); // forward(10cm), turn -90
	void onEnteringAusrichten_2();
	void onEnteringAusrichten_3();
	void onEnteringOnTrail();// forward
	void onEnteringOffTrail();// stop
	void onEnteringStopping();// ecken_cnt()
	void onEnteringCorrectTrailRight(); // turn()
	void onEnteringCorrectTrailLeft(); // turn()
	void onEnteringLampeGray();

	void onEnteringRelay();
	void onEnteringEnde();
	void onEnteringReady();

	void onEnteringFinal();

	/* -- Methods called when leaving a state. */
	void onLeavingStart();
	void onLeavingSuchen(); //stop

	void onLeavingAusrichten(); // Stop
	void onLeavingAusrichten_2();
	void onLeavingAusrichten_3();

	void onLeavingOnTrail();
	void onLeavingStopping();
	void onLeavingOffTrail();
	void onLeavingFailure();
	void onLeavingLampeGray();

	void onLeavingRelay();
	void onLeavingEnde();
	void onLeavingReady();


	void onLeavingCorrectTrailLeft(); // stop
	void onLeavingCorrectTrailRight();// stop
	void onLeavingFinal();


	/* --Sets the state explicitly. */
	void state(State s);
	/* --Process state transition; returns new state. */
	void transition(Event ev);

	/* --Constant names for debugging purpose. */
	static const std::map<State, std::string> StateDescription;
	static const std::map<Event, std::string> EventDescription;

};
#endif
#pragma once
