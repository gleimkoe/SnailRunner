#ifndef STARTLAUFERMACHINE_H
#define STARTLAUFERMACHINE_H

#include <map>
#include <string>

#include "WaitUntil.h"

/* --Forward declaration. */
class SnailRunner;

class StartLauferMachine {
public:

<<<<<<< HEAD
	enum class State {	FAILURE, START, SUCHEN, AUSRICHTEN, AUSRICHTEN_2, AUSRICHTEN_3, AUSRICHTEN_4, CORRECT_TRAIL_LEFT, CORRECT_TRAIL_RIGHT, ON_TRAIL, OFF_TRAIL, STOPPING, RELAY, LAMPE_GRAY, ENDE, READY,

						OBSTACLE_DETECTED, EVASION_1, EVASION_2, EVASION_3, EVASION_4, EVASION_5, EVASION_6, BACK_1, BACK_2, ALLIGN_1, ALLIGN_2, CORRECT_TO_LEFT, CORRECT_TO_RIGHT, // STOPPING_GREY

						VOR_FINAL, FINAL };

	enum class Event { WALL_AHEAD, NOT_WALL_AHEAD, OFF_TRAIL, ON_TRAIL, IS_STOPPED, ON_GREY, 
						LICHT_HINTEN, NOT_LICHT_HINTEN, NO_SIDEWALL, // OFF_GREY 
						TOO_CLOSE, TOO_FAR };
=======
	enum class State {
		FAILURE, START, SUCHEN, AUSRICHTEN, AUSRICHTEN_2, AUSRICHTEN_3, AUSRICHTEN_4, CORRECT_TRAIL_LEFT, CORRECT_TRAIL_RIGHT, ON_TRAIL, OFF_TRAIL, STOPPING, RELAY, LAMPE_GRAY, ENDE, READY,

		OBSTACLE_DETECTED, EVASION_1, EVASION_2, EVASION_3, EVASION_4, EVASION_5, EVASION_6, BACK_1, BACK_2, ALLIGN_1, ALLIGN_2, CORRECT_TO_LEFT, CORRECT_TO_RIGHT,

					   VOR_FINAL, FINAL };

	enum class Event { WALL_AHEAD, NOT_WALL_AHEAD, OFF_TRAIL, ON_TRAIL, IS_STOPPED, ON_GREY, LICHT_HINTEN, NOT_LICHT_HINTEN, NO_SIDEWALL, TOO_CLOSE, TOO_FAR };
>>>>>>> 36c5cd54ae53c7930c1fde1bf0bba0e2793dab75

	StartLauferMachine(SnailRunner* r);
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
	int count = 0;
	


	/* -- Methods called when entering a state. */
	void onEnteringFailure();
	void onEnteringStart(); // wait
	void onEnteringSuchen(); //forward 
	void onEnteringAusrichten(); // forward(10cm), turn -90
	void onEnteringAusrichten_2();
	void onEnteringAusrichten_3();
	void onEnteringAusrichten_4();
	void onEnteringOnTrail();// forward
	void onEnteringOffTrail();// stop
	void onEnteringStopping();// ecken_cnt()
	void onEnteringCorrectTrailRight(); // turn()
	void onEnteringCorrectTrailLeft(); // turn()
	void onEnteringLampeGray();

	void onEnteringRelay();
	void onEnteringEnde();
	void onEnteringReady();

	void onEnteringObstacleDetected();
	void onEnteringEvasion1();
	void onEnteringEvasion2();
	void onEnteringEvasion3();
	void onEnteringEvasion4();
	void onEnteringEvasion5();
	void onEnteringEvasion6();
	void onEnteringBack1();
	void onEnteringBack2();
	void onEnteringAllign1();
	void onEnteringAllign2();
	void onEnteringCorrectToLeft();
	void onEnteringCorrectToRight();
<<<<<<< HEAD
	void onEnteringStoppingGrey();

=======
>>>>>>> 36c5cd54ae53c7930c1fde1bf0bba0e2793dab75

	void onEnteringVorFinal();
	void onEnteringFinal();

	/* -- Methods called when leaving a state. */
	void onLeavingStart();
	void onLeavingSuchen(); //stop

	void onLeavingAusrichten(); // Stop
	void onLeavingAusrichten_2();
	void onLeavingAusrichten_3();
	void onLeavingAusrichten_4();

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

	void onLeavingObstacleDetected();
	void onLeavingEvasion1();
	void onLeavingEvasion2();
	void onLeavingEvasion3();
	void onLeavingEvasion4();
	void onLeavingEvasion5();
	void onLeavingEvasion6();
	void onLeavingBack1();
	void onLeavingBack2();
	void onLeavingAllign1();
	void onLeavingAllign2();
	void onLeavingCorrectToLeft();
	void onLeavingCorrectToRight();
<<<<<<< HEAD
	void onLeavingStoppingGrey();
=======
>>>>>>> 36c5cd54ae53c7930c1fde1bf0bba0e2793dab75

	void onLeavingVorFinal();
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
