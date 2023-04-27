#include "StartLauferMachine.h"
#include "SnailRunner.h"

/* --Initialize State Description. */



const std::map<StartLauferMachine::State, std::string> StartLauferMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::START, "START" },
	{ State::SUCHEN, "SUCHEN" },
	{ State::AUSRICHTEN, "AUSRICHTEN" },
	{ State::AUSRICHTEN_2, "AUSRICHTEN_2" },
	{ State::AUSRICHTEN_3, "AUSRICHTEN_3" },
	{ State::CORRECT_TRAIL_LEFT, "CORRECT_TRAIL_LEFT" },
	{ State::CORRECT_TRAIL_RIGHT, "CORRECT_TRAIL_RIGHT" },
	{ State::ON_TRAIL, "ON_TRAIL" },
	{ State::OFF_TRAIL, "OFF_TRAIL" },
	{ State::STOPPING, "STOPPING" },
	{ State::ON_ENDE, "ON_ENDE" },
	{ State::LAMPE_GRAY, "LAMPE_GRAY" },
	{ State::ENDE, "ENDE" },
	{State::READY, "READY"},
	{State::READY_2, "READY_2"},
	{ State::STOPPED, "STOPPED" },
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
	

	{ Event::RUNDE_CNT, "RUNDE_CNT" },
	{Event::NOT_RUNDE_CNT, "NOT_RUNDE_CNT"},
	{Event::LICHT_HINTEN, "LICHT_HINTEN"},
	{Event::NOT_LICHT_HINTEN, "NOT_LICHT_HINTEN"}
	
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
	ecke = 0;
	runde = 0;
	/* --Start with initial transition. */
	onEnteringStart();
	/* --INFO: Here you can change/add initial values when state machine is started. */
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
		case StartLauferMachine::Event:: RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
			break;
		case StartLauferMachine::Event::ON_GREY: onLeavingAusrichten_2(); onEnteringAusrichten_3();
			break;
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default:onLeavingAusrichten_2(); onEnteringFailure();
			break;
		}

	case State::AUSRICHTEN_3:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL: onLeavingAusrichten_3(); onEnteringOnTrail();
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default:onLeavingAusrichten_3(); onEnteringFailure();
			break;
		}

	case State::ON_TRAIL:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD:
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD:
			break;
		case StartLauferMachine::Event::OFF_TRAIL:onLeavingOnTrail(); onEnteringOffTrail();
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:onLeavingOnTrail(); onEnteringOnTrail();
			break;
		case StartLauferMachine::Event::ON_GREY: onLeavingOnTrail(); onEnteringOnEnde();
			break;
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onEnteringFailure();
		}
		break;

	case State::ON_ENDE:
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
		case StartLauferMachine::Event::RUNDE_CNT: onLeavingOnEnde(); onEnteringEnde();
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:onLeavingOnEnde(); onEnteringLampeGray();
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		default: onEnteringFailure();
			break;
		}

	case State::LAMPE_GRAY:
		switch (ev)
		{
		case StartLauferMachine::Event::WALL_AHEAD: onLeavingLampeGray(); onEnteringLampeGray();
			break;
		case StartLauferMachine::Event::NOT_WALL_AHEAD: onLeavingLampeGray(); onEnteringReady();
			break;
		case StartLauferMachine::Event::OFF_TRAIL:
			break;
		case StartLauferMachine::Event::ON_TRAIL:
			break;
		case StartLauferMachine::Event::IS_STOPPED:
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onEnteringFailure();
			break;
		}
	case State::READY:
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
		case StartLauferMachine::Event::IS_STOPPED: onLeavingReady(); onEnteringReady_2();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN: 
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default: onEnteringFailure();
			break;
		}

	case State::READY_2:
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN: onLeavingReady_2(); onEnteringAusrichten_3();
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN: onLeavingReady_2(); onEnteringReady_2();
			break;
		default:
			break;
		}

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
		case StartLauferMachine::Event::IS_STOPPED: onLeavingEnde(); onEnteringFinal();
			break;
		case StartLauferMachine::Event::ON_GREY:
			break;
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
			break;
		case StartLauferMachine::Event::LICHT_HINTEN:
			break;
		case StartLauferMachine::Event::NOT_LICHT_HINTEN:
			break;
		default:onLeavingEnde(); onEnteringFailure();
			break;
		}
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
		case StartLauferMachine::Event::RUNDE_CNT:
			break;
		case StartLauferMachine::Event::NOT_RUNDE_CNT:
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

void StartLauferMachine::onEnteringAusrichten_2() { // turn(360)
	state(State::AUSRICHTEN_2);
	robot->turn(-180);
	cout << "Ausrichten_2" << endl;
}

void StartLauferMachine::onEnteringAusrichten_3() {
	state(State::AUSRICHTEN_3);
	robot->forward(1, METER);
	cout << "Ausrichten_3" << endl;
}

void StartLauferMachine::onEnteringOnTrail() {// forward
	state(State::ON_TRAIL);
	robot->forward(1, METER);
	count = 1;
	cout << "OnTrail" << endl;
}

void StartLauferMachine::onEnteringOffTrail() {// stop
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
		ecke++;
		cout << "Ecke Nr. : " << ecke << endl;
	}
	robot->turn(count * 7.5);
	cout << "CorrectRight" << endl;
}
void StartLauferMachine::onEnteringCorrectTrailLeft() { // turn()
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


void StartLauferMachine::onEnteringFinal() {
	state(State::FINAL);
	robot->lampright().on();
	cout << "Final" << endl;
}

void StartLauferMachine::onEnteringLampeGray() {
	state(State::LAMPE_GRAY);
	robot->stop();
	robot->lampfront().on();
	cout << "LampeGray" << endl;
}

void StartLauferMachine::onEnteringReady() {
	state(State::READY);
	robot->forward(0.2, METER);
	robot->lampright().off();
	robot->lampfront().off();
	cout << "Ready" << endl;
}

void StartLauferMachine::onEnteringOnEnde() {
	state(State::ON_ENDE);
	robot->stop();
	cout << "OnEnde" << endl;
}

void StartLauferMachine::onEnteringEnde() {
	state(State::ENDE);
	robot->forward(0.2, METER);
	robot->lampright().off();
	robot->lampfront().off();
	cout << "Ende" << endl;
}

void StartLauferMachine::onEnteringReady_2() {
	state(State::READY_2);
	cout << "Ready_2" << endl;
}



void StartLauferMachine::onLeavingAusrichten() {} // Stop
	//robot->stop();

void StartLauferMachine::onLeavingAusrichten_2() {
	robot->stop();
	cout << "AUSRICHTEN_2 STOP" << endl;
}

void StartLauferMachine::onLeavingAusrichten_3() {}
void StartLauferMachine::onLeavingOnTrail() {}
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

void StartLauferMachine::onLeavingStart() {}
void StartLauferMachine::onLeavingSuchen() { // Stop
	robot->stop();
}

void StartLauferMachine::onLeavingLampeGray() {
	
}

void StartLauferMachine::onLeavingOnEnde() {
	robot->forward(0.1, METER);
}

void StartLauferMachine::onLeavingEnde() {}

void StartLauferMachine::onLeavingReady() {}

void StartLauferMachine::onLeavingReady_2(){}