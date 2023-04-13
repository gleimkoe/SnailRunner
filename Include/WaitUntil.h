#ifndef WAITUNTIL_H_
#define WAITUNTIL_H_

#include "Makros.h"
#include "Button.h"
#include "Sensor.h"
#include "Contact.h"
#include "Photo.h"

#include <windows.h>

DLL_DECLSPEC void WaitUntilIsOver(int timeout);

DLL_DECLSPEC void WaitUntilIsPressed(Button& button, int spin = 100);

DLL_DECLSPEC void WaitUntilIsContacted(Contact& contact, int spin = 100);

DLL_DECLSPEC void WaitUntilIsDark(Photo& photo, int spin = 100);


#endif
