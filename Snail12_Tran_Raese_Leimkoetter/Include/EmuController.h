///////////////////////////////////////////////////////////
//  EmuController.h
//  Implementation of the Class EmuController
//  Created on:      21-Mai-2010 11:53:24
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_8A7E833C_149D_4f1e_8211_CEBE1BE00432__INCLUDED_)
#define EA_8A7E833C_149D_4f1e_8211_CEBE1BE00432__INCLUDED_

#include "Makros.h"
#include "EmulationType.h"
#include "Controller.h"

class DLL_DECLSPEC EmuController : public Controller
{

public:
	EmuController();
	virtual ~EmuController();

	virtual bool input(InputPort iPort, int& nValue, UniversalInput iType = ANALOG_5KOMEGA);
	virtual bool output(MotorPort oport, Direction dir, int speed);
	virtual bool output(OutputPort oport, int value);
	virtual bool clear(InputPort cPort);
	virtual bool configure(InputPort iPort, UniversalInput iType = ANALOG_5KOMEGA);

	void set(EmulationType et);

private:
	EmulationType itype;

};
#endif // !defined(EA_8A7E833C_149D_4f1e_8211_CEBE1BE00432__INCLUDED_)
