#include <windows.h>
#include <synchapi.h>

#include <map>
#include "educationline.h"

/* --Statische Map zum Merken der letzt ermittelten Werte. */
static std::map<InputPort, int> lastPressed, lastReleased;

void EducationLine::sleep(unsigned int ms) {
	Sleep(ms);
}
/* --Alternative Implementierung. */

#include "RbController.h"

RbController* rb = nullptr;

void EducationLine::initialize() {
	if (!rb) {
		rb = new RbController;
		if (rb->open(0)==false)
			throw CrossRoadsFailure();
	}
}

void EducationLine::enable(LED led) {
	if (rb) {
		OutputPort port=static_cast<OutputPort>(led);
		rb->output(port, 1);
	}
	else
		throw CrossRoadsFailure();
}

void EducationLine::disable(LED led) {
	if (rb) {
		OutputPort port = static_cast<OutputPort>(led);
		rb->output(port, 0);
	}
	else
		throw CrossRoadsFailure();
}

bool EducationLine::isPressed(EducationLine::Interaction button) {
	if (rb) {
		InputPort port = static_cast<InputPort>(button);
		int value = 0;
		if (rb->input(port, value, DIGITAL_5KOMEGA)) {
			bool retvalue = false;
			/* --Prüfe, ob Port bereits abgefragt wurde. */
			if (lastPressed.find(port) != lastPressed.end()) {
				/* --Prüfe, ob Veränderung zum vorherigen Wert stattgefunden hat. */
				if (value != 0 && lastPressed[port] == 0) {
//					std::cout << "Pressed " << port << "=" << value << "," << lastPressed[port] << std::endl;
					retvalue = true;
				}
			}
			/* --Aktuellen Wert jetzt merken. */
			lastPressed[port] = value;
			return retvalue;
		}
		else
			throw CrossRoadsFailure();
	}
	else
		throw CrossRoadsFailure();
}

bool EducationLine::isReleased(EducationLine::Interaction button) {
	if (rb) {
		InputPort port = static_cast<InputPort>(button);
		int value = 0;
		if (rb->input(port, value, DIGITAL_5KOMEGA)) {
			bool retvalue = false;
			/* --Prüfe, ob Port bereits abgefragt wurde. */
			if (lastReleased.find(port) != lastReleased.end()) {
				/* --Prüfe, ob Veränderung zum vorherigen Wert stattgefunden hat. */
				if (value == 0 && lastReleased[port] != 0) {
//					std::cout << "Released " << port << "=" << value << "," << lastReleased[port] << std::endl;
					retvalue = true;
				}
			}
			/* --Aktuellen Wert jetzt merken. */
			lastReleased[port] = value;
			return retvalue;
		}
		else
			throw CrossRoadsFailure();
	}
	else
		throw CrossRoadsFailure();
}

int EducationLine::brightness() {
	if (rb) {
		int value=-1;
		if (rb->input(A1, value, ANALOG_10KV))
			return value;
		else
			throw CrossRoadsFailure();
	}
	else
		throw CrossRoadsFailure();
}


void EducationLine::test() {
	/* --Alle LEDS einmal durchschalten. */

	/* --Zuerst die Verkehrsampeln (rechts-oben-links-unten).*/
	VLED traffic = {
		LED::MAIN_ROAD_RIGHT_RED, LED::MAIN_ROAD_RIGHT_YELLOW, LED::MAIN_ROAD_RIGHT_GREEN,
		LED::SIDE_ROAD_TOP_RED, LED::SIDE_ROAD_TOP_YELLOW, LED::SIDE_ROAD_TOP_GREEN,
		LED::MAIN_ROAD_LEFT_RED, LED::MAIN_ROAD_LEFT_YELLOW, LED::MAIN_ROAD_LEFT_GREEN,
		LED::SIDE_ROAD_BOTTOM_RED, LED::SIDE_ROAD_BOTTOM_YELLOW, LED::SIDE_ROAD_BOTTOM_GREEN};

	for (auto t : traffic) {
		enable(t);
		sleep(1000);
		disable(t);
		sleep(1000);
	}

	/* --Jetzt die Fussgängerampeln (rechts-oben-links-unten). */
	VLED pedestrian = {
		LED::MAIN_ROAD_RIGHT_TOP_RED, LED::MAIN_ROAD_RIGHT_TOP_GREEN,
		LED::FLASH,
		LED::SIDE_ROAD_TOP_RIGHT_RED, LED::SIDE_ROAD_TOP_RIGHT_GREEN,
		LED::SIDE_ROAD_TOP_LEFT_RED, LED::SIDE_ROAD_TOP_LEFT_GREEN,
		LED::MAIN_ROAD_LEFT_TOP_RED, LED::MAIN_ROAD_LEFT_TOP_GREEN,
		LED::LIGHTING,
		LED::MAIN_ROAD_LEFT_BOTTOM_RED, LED::MAIN_ROAD_LEFT_BOTTOM_GREEN,
		LED::SIDE_ROAD_BOTTOM_LEFT_RED, LED::SIDE_ROAD_BOTTOM_LEFT_GREEN,
		LED::SIDE_ROAD_BOTTOM_RIGHT_RED, LED::SIDE_ROAD_BOTTOM_RIGHT_GREEN,
		LED::MAIN_ROAD_RIGHT_BOTTOM_RED, LED::MAIN_ROAD_RIGHT_BOTTOM_GREEN
	};

	for (auto p : pedestrian) {
		enable(p);
		sleep(1000);
		disable(p);
		sleep(1000);
	}
}

void EducationLine::test2() {

	std::vector<EducationLine::Interaction> ivec = {
		Interaction::MAIN_ROAD_RIGHT_TOP,
		Interaction::MAIN_ROAD_RIGHT_BOTTOM,
		Interaction::MAIN_ROAD_LEFT_TOP,
		Interaction::MAIN_ROAD_LEFT_BOTTOM,
		Interaction::SIDE_ROAD_TOP_RIGHT,
		Interaction::SIDE_ROAD_TOP_LEFT,
		Interaction::SIDE_ROAD_BOTTOM_RIGHT,
		Interaction::SIDE_ROAD_BOTTOM_LEFT,
		Interaction::MAIN_ROAD_RIGHT_CAR,
		Interaction::MAIN_ROAD_LEFT_CAR,
		Interaction::SIDE_ROAD_TOP_CAR,
		Interaction::SIDE_ROAD_BOTTOM_CAR,
		Interaction::CENTER_LEFT_CAR,
		Interaction::CENTER_RIGHT_CAR
	};

	for (;;) {
		Interaction value = input();
		if (value == Interaction::CENTER_RIGHT_CAR)
			return;
		else {
			enable(LED::FLASH);
			std::cout << static_cast<int>(value) << "... " << std::flush;
		}
	}
}

EducationLine::Interaction EducationLine::input() {
	/* --Eingabequellen. */
	std::vector<EducationLine::Interaction> ivec = {
		Interaction::MAIN_ROAD_RIGHT_TOP,
		Interaction::MAIN_ROAD_RIGHT_BOTTOM,
		Interaction::MAIN_ROAD_LEFT_TOP,
		Interaction::MAIN_ROAD_LEFT_BOTTOM,
		Interaction::SIDE_ROAD_TOP_RIGHT,
		Interaction::SIDE_ROAD_TOP_LEFT,
		Interaction::SIDE_ROAD_BOTTOM_RIGHT,
		Interaction::SIDE_ROAD_BOTTOM_LEFT,
		Interaction::MAIN_ROAD_RIGHT_CAR,
		Interaction::MAIN_ROAD_LEFT_CAR,
		Interaction::SIDE_ROAD_TOP_CAR,
		Interaction::SIDE_ROAD_BOTTOM_CAR,
		Interaction::CENTER_LEFT_CAR,
		Interaction::CENTER_RIGHT_CAR
	};

	/* --Busy Wait. */
	for (;;) {
		/* --Check if pressed. */
		for (auto i : ivec) {
			if (isPressed(i)) {
				/* --Warten bis wieder losgelassen wird. */
				while (!isReleased(i)) sleep(25);
				return i;
			}
		}
		/* --Augenblick pausieren. */
		sleep(25);
	}
}

const EducationLine::CrosswalkDefinition EducationLine::LeftDefinition = { 
	EducationLine::LED::MAIN_ROAD_LEFT_RED,
	EducationLine::LED::MAIN_ROAD_LEFT_YELLOW,
	EducationLine::LED::MAIN_ROAD_LEFT_GREEN,
	EducationLine::Interaction::MAIN_ROAD_LEFT_CAR,
	EducationLine::LED::MAIN_ROAD_LEFT_TOP_RED,
	EducationLine::LED::MAIN_ROAD_LEFT_TOP_GREEN,
	EducationLine::LED::MAIN_ROAD_LEFT_BOTTOM_RED,
	EducationLine::LED::MAIN_ROAD_LEFT_BOTTOM_GREEN,
	EducationLine::Interaction::MAIN_ROAD_LEFT_TOP,
	EducationLine::Interaction::MAIN_ROAD_LEFT_BOTTOM
};

const EducationLine::CrosswalkDefinition EducationLine::RightDefinition = {
	EducationLine::LED::MAIN_ROAD_RIGHT_RED,
	EducationLine::LED::MAIN_ROAD_RIGHT_YELLOW,
	EducationLine::LED::MAIN_ROAD_RIGHT_GREEN,
	EducationLine::Interaction::MAIN_ROAD_RIGHT_CAR,
	EducationLine::LED::MAIN_ROAD_RIGHT_TOP_RED,
	EducationLine::LED::MAIN_ROAD_RIGHT_TOP_GREEN,
	EducationLine::LED::MAIN_ROAD_RIGHT_BOTTOM_RED,
	EducationLine::LED::MAIN_ROAD_RIGHT_BOTTOM_GREEN,
	EducationLine::Interaction::MAIN_ROAD_RIGHT_TOP,
	EducationLine::Interaction::MAIN_ROAD_RIGHT_BOTTOM
};

const EducationLine::CrosswalkDefinition EducationLine::TopDefinition = {
	EducationLine::LED::SIDE_ROAD_TOP_RED,
	EducationLine::LED::SIDE_ROAD_TOP_YELLOW,
	EducationLine::LED::SIDE_ROAD_TOP_GREEN,
	EducationLine::Interaction::SIDE_ROAD_TOP_CAR,
	EducationLine::LED::SIDE_ROAD_TOP_RIGHT_RED,
	EducationLine::LED::SIDE_ROAD_TOP_RIGHT_GREEN,
	EducationLine::LED::SIDE_ROAD_TOP_LEFT_RED,
	EducationLine::LED::SIDE_ROAD_TOP_LEFT_GREEN,
	EducationLine::Interaction::SIDE_ROAD_TOP_RIGHT,
	EducationLine::Interaction::SIDE_ROAD_TOP_LEFT
};

const EducationLine::CrosswalkDefinition EducationLine::BottomDefinition = {
	EducationLine::LED::SIDE_ROAD_BOTTOM_RED,
	EducationLine::LED::SIDE_ROAD_BOTTOM_YELLOW,
	EducationLine::LED::SIDE_ROAD_BOTTOM_GREEN,
	EducationLine::Interaction::SIDE_ROAD_BOTTOM_CAR,
	EducationLine::LED::SIDE_ROAD_BOTTOM_RIGHT_RED,
	EducationLine::LED::SIDE_ROAD_BOTTOM_RIGHT_GREEN,
	EducationLine::LED::SIDE_ROAD_BOTTOM_LEFT_RED,
	EducationLine::LED::SIDE_ROAD_BOTTOM_LEFT_GREEN,
	EducationLine::Interaction::SIDE_ROAD_BOTTOM_RIGHT,
	EducationLine::Interaction::SIDE_ROAD_BOTTOM_LEFT
};

