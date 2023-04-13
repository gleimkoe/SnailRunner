#include "StartStateMachine.h"
#include "SnailRunner.h"

/* --Initialize State Description. */

const std::map<StartStateMachine::State, std::string> StartStateMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::START, "START" },
	{ State::SUCHEN, "SUCHEN" },
	{ State::STOPPING, "STOPPING" },
	{ State::CORRECT_TRAIL_LEFT, "CORRECT_TRAIL_LEFT" },
	{ State::CORRECT_TRAIL_RIGHT, "CORRECT_TRAIL_RIGHT" },
	{ State::AUSRICHTEN, "AUSRICHTEN" },
	{ State::OFF_TRAIL, "OFF_TRAIL" },
	{ State::ON_TRAIL, "ON_TRAIL" },
	{ State::FINAL, "FINAL" },
	{ State::LAMPE_STOP, "LAMPE_STOP" }
	/* --INFO: Here you should add new states for debugging purpose. */
};

const std::map<StartStateMachine::Event, std::string> StartStateMachine::EventDescription = {
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::ON_TRAIL, "ON_TRAIL" },
	{ Event::OFF_TRAIL, "OFF_TRAIL" },
	{ Event::WALL_AHEAD, "WALL_AHEAD" },
	{ Event::NOT_WALL_AHEAD, "NOT_WALL_AHEAD" },
	{ Event::ECKEN_CNT, "ECKEN_CNT" }

	/* --INFO: Here you should add new events for debugging purpose. */
};

StartStateMachine::StartStateMachine(SnailRunner* r) :mystate(State::FAILURE), robot(r), count(1) {}

/* --INFO: Do not touch this method. */
void StartStateMachine::state(State s) {
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
StartStateMachine::State StartStateMachine::state() const { return mystate; }

void StartStateMachine::start() {
	/* --Log string. */
	robot->controller()->annotate("RE/START START STATE MACHINE");
	/* --Set everything to initial values. */
	count = 1;
	ecke = 0;
	/* --Start with initial transition. */
	onEnteringStart();
	/* --INFO: Here you can change/add initial values when state machine is started. */
}

void StartStateMachine::restart() {
	/* --Same as start. */
	start();
}

/* --INFO: Do not touch this method. */
void StartStateMachine::handle(Event ev) {
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

/* --INFO: This is the implemented state machine. Add/Modify/Enhance this method for additional functionality. */
void StartStateMachine::transition(Event ev) {
	/* --State machine as nested switch case. */
	switch (mystate) {
	case State::START:
		switch (ev) {
		case StartStateMachine::Event::WALL_AHEAD:onLeavingStart(); onEnteringStart();
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:onLeavingStart(); onEnteringSuchen();
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::SUCHEN:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:onLeavingSuchen(); onEnteringSuchen();
			break;
		case StartStateMachine::Event::ON_TRAIL:onLeavingSuchen(); onEnteringAusrichten();
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::AUSRICHTEN:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:onLeavingAusrichten(); onEnteringOnTrail();
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::ON_TRAIL:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:onLeavingOnTrail(); onEnteringOffTrail();
			break;
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:onLeavingOnTrail(); onEnteringOnTrail();
			break;
		default: onEnteringFailure();
		}
		break;

	case State::OFF_TRAIL:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED: onLeavingOffTrail(); onEnteringCorrectTrailLeft();
			break;
		default: onEnteringFailure();
		}
		break;

	case State::STOPPING:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT: onLeavingStopping(); onEnteringLampeStop();
			break;
		case StartStateMachine::Event::IS_STOPPED: onLeavingStopping(); onEnteringOnTrail();
			break;
		default: onEnteringFailure();
		}
		break;

	case State::CORRECT_TRAIL_LEFT:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:onLeavingCorrectTrailLeft(); onEnteringStopping();
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED: onLeavingCorrectTrailLeft(); onEnteringCorrectTrailRight();
			break;
		default: onEnteringFailure();
		}
		break;

	case State::CORRECT_TRAIL_RIGHT:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL: onLeavingCorrectTrailRight(); onEnteringStopping();
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED: onLeavingCorrectTrailRight(); onEnteringCorrectTrailLeft();
			break;
		default: onEnteringFailure();
		}
		break;

	case State::LAMPE_STOP:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED: onLeavingLampeStop(); onEnteringFinal();
			break;
		default: onEnteringFailure();
		}
		break;

	case State::FINAL:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:
			break;
		default:;
		}
		break;

	default: state(State::FAILURE);
	};
}

void StartStateMachine::onEnteringFailure() {
	state(State::FAILURE);
	robot -> stop();
	robot->lampleft().on();
}

void StartStateMachine::onEnteringStart() { // wait
	state(State::START);
}

void StartStateMachine::onEnteringSuchen() { //forward 
	state(State::SUCHEN);
	robot->forward(1, METER);
	count = 1;
}

void StartStateMachine::onEnteringAusrichten() { // forward(10cm), turn -90
	state(State::AUSRICHTEN);
	robot->forward(0.15, METER);
	WaitUntilIsOver(3000);
	robot->turn(-90);
}

void StartStateMachine::onEnteringOnTrail() {// forward
	state(State::ON_TRAIL);
	robot->forward(1, METER);
	count = 1;
}

void StartStateMachine::onEnteringOffTrail() {// stop
	state(State::OFF_TRAIL);
	robot->stop();
}

void StartStateMachine::onEnteringStopping() {// ecken_cnt()
	state(State::STOPPING);

}

void StartStateMachine::onEnteringCorrectTrailRight() { // turn()
	state(State::CORRECT_TRAIL_RIGHT);
	count *= 2;
	if (count == 8)
	{
		ecke++;
		cout << "Ecke Nr. : " << ecke;
	}
	robot->turn(count * 15);
}
void StartStateMachine::onEnteringCorrectTrailLeft() { // turn()
	state(State::CORRECT_TRAIL_LEFT);
	count *= 2;
	if (count == 8)
	{
		ecke++;
		cout << "Ecke Nr. : " << ecke;
	}
	robot->turn(count * -15);
}
void StartStateMachine::onEnteringLampeStop() {//lamp.on
	state(State::LAMPE_STOP);
	robot->stop();
	robot->lampright().on();
}
void StartStateMachine::onEnteringFinal() {
	state(State::FINAL);
	robot->lampright().on();
}

void StartStateMachine::onLeavingAusrichten(){ // Stop
	robot->stop();
}
void StartStateMachine::onLeavingOnTrail() {}
void StartStateMachine::onLeavingStopping() {}
void StartStateMachine::onLeavingOffTrail(){
	robot->forward(0.1, METER);
}
void StartStateMachine::onLeavingFailure() {}
void StartStateMachine::onLeavingLampeStop() {}

void StartStateMachine::onLeavingCorrectTrailLeft() { // stop
	robot->stop();
}
void StartStateMachine::onLeavingCorrectTrailRight() {// stop
	robot->stop();
}
void StartStateMachine::onLeavingFinal() {}

void StartStateMachine::onLeavingStart() {}
void StartStateMachine::onLeavingSuchen() { // Stop
	
	robot->stop();
}