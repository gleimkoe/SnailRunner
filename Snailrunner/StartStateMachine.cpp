#include "StartStateMachine.h"
#include "SnailRunner.h"

/* --Initialize State Description. */

const std::map<StartStateMachine::State, std::string> StartStateMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::START, "START" },
	{ State::SUCHEN, "SUCHEN" },
	{ State::AUSRICHTEN, "AUSRICHTEN" },
	{ State::AUSRICHTEN_2, "AUSRICHTEN_2" },
	{ State::AUSRICHTEN_3, "AUSRICHTEN_3" },
	{ State::LAMPE_STOP, "LAMPE_STOP" },
	{ State::CORRECT_TRAIL_LEFT, "CORRECT_TRAIL_LEFT" },
	{ State::CORRECT_TRAIL_RIGHT, "CORRECT_TRAIL_RIGHT" },
	{ State::ON_TRAIL, "ON_TRAIL" },
	{ State::OFF_TRAIL, "OFF_TRAIL" },
	{ State::STOPPING, "STOPPING" },
	{ State::FINAL, "FINAL" },

	/* --INFO: Here you should add new states for debugging purpose. */
};

const std::map<StartStateMachine::Event, std::string> StartStateMachine::EventDescription = {
	{ Event::WALL_AHEAD, "WALL_AHEAD" },
	{ Event::NOT_WALL_AHEAD, "NOT_WALL_AHEAD" },
	{ Event::OFF_TRAIL, "OFF_TRAIL" },
	{ Event::ON_TRAIL, "ON_TRAIL" },
	{ Event::ECKEN_CNT, "ECKEN_CNT" },
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::ON_GREY, "ON_GREY" }
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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:
			break;
		case StartStateMachine::Event::ON_GREY: onLeavingSuchen(); onEnteringAusrichten();
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
		case StartStateMachine::Event::ON_TRAIL:
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED: onLeavingAusrichten(); onEnteringAusrichten_2();
			break;
		case StartStateMachine::Event::ON_GREY: 
			break;
		default: onEnteringFailure();
		}
		break;

	case State::AUSRICHTEN_2:
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
		case StartStateMachine::Event::ON_GREY:onLeavingAusrichten_2(); onEnteringAusrichten_3();
			break;
		default: onEnteringFailure();
			break;
		}

	case State::AUSRICHTEN_3:
		switch (ev)
		{
		case StartStateMachine::Event::WALL_AHEAD:
			break;
		case StartStateMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartStateMachine::Event::OFF_TRAIL:
			break;
		case StartStateMachine::Event::ON_TRAIL: onLeavingAusrichten_3(); onEnteringOnTrail();
			break;
		case StartStateMachine::Event::ECKEN_CNT:
			break;
		case StartStateMachine::Event::IS_STOPPED:
			break;
		case StartStateMachine::Event::ON_GREY:
			break;
		default: onEnteringFailure();
			break;
		}

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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_GREY:
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
		case StartStateMachine::Event::ON_GREY:
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
	cout << "Start" << endl;
}

void StartStateMachine::onEnteringSuchen() { //forward 
	state(State::SUCHEN);
	robot->forward(1, METER);
	count = 1;
	cout << "Suchen" << endl;
}

void StartStateMachine::onEnteringAusrichten() { // forward(10cm)
	state(State::AUSRICHTEN);
	robot->forward(0.1, METER);
	
	cout << "Ausrichten" << endl;
}

void StartStateMachine::onEnteringAusrichten_2() { // turn(360)
	state(State::AUSRICHTEN_2);
	robot->turn(-180);
	cout << "Ausrichten_2" << endl;
}

void StartStateMachine::onEnteringAusrichten_3() {
	state(State::AUSRICHTEN_3);
	robot->forward(1, METER);
	cout << "Ausrichten_3" << endl;
}

void StartStateMachine::onEnteringOnTrail() {// forward
	state(State::ON_TRAIL);
	robot->forward(1, METER);
	count = 1;
	cout << "OnTrail" << endl;
}

void StartStateMachine::onEnteringOffTrail() {// stop
	state(State::OFF_TRAIL);
	robot->stop();
	cout << "OffTrail" << endl;
}

void StartStateMachine::onEnteringStopping() {// ecken_cnt()
	state(State::STOPPING);
	cout << "Stopping" << endl;

}

void StartStateMachine::onEnteringCorrectTrailRight() { // turn()
	state(State::CORRECT_TRAIL_RIGHT);
	count *= 2;
	if (count == 16)
	{
		ecke++;
		cout << "Ecke Nr. : " << ecke << endl;
	}
	robot->turn(count * 7.5);
	cout << "CorrectRight" << endl;
}
void StartStateMachine::onEnteringCorrectTrailLeft() { // turn()
	state(State::CORRECT_TRAIL_LEFT);
	count *= 2;
	if (count == 16)
	{
		ecke++;
		cout << "Ecke Nr. : " << ecke << endl;
	}
	robot->turn(count * -7.5);
	cout << "CorrectLeft" << endl;
}
void StartStateMachine::onEnteringLampeStop() {//lamp.on
	state(State::LAMPE_STOP);
	robot->stop();
	robot->lampright().on();
	cout << "LampeStop" << endl;
}
void StartStateMachine::onEnteringFinal() {
	state(State::FINAL);
	robot->lampright().on();
	cout << "Final" << endl;
}

void StartStateMachine::onLeavingAusrichten() {} // Stop
	//robot->stop();

void StartStateMachine::onLeavingAusrichten_2() {
	robot->stop();
	cout << "AUSRICHTEN_2 STOP" << endl;
}

void StartStateMachine::onLeavingAusrichten_3() {}
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