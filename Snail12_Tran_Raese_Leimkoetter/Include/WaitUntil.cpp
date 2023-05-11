#include "WaitUntil.h"

void WaitUntilIsOver(int timeout) {
	Sleep(timeout);
}

void WaitUntilIsPressed(Button& button, int spin) {
	while (!button.isPressed())
		Sleep(spin);
}

void WaitUntilIsContacted(Contact& contact, int spin) {
	while (!contact.isContacted())
		Sleep(spin);
}

void WaitUntilIsDark(Photo& photo, int spin) {
	while (!photo.isDark())
		Sleep(spin);
}