///////////////////////////////////////////////////////////
//  Motor.h
//  Implementation of the Class Motor
//  Created on:      21-Mai-2010 11:53:26
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_D8153474_5248_40bc_A5F0_2E1CC65CC2FA__INCLUDED_)
#define EA_D8153474_5248_40bc_A5F0_2E1CC65CC2FA__INCLUDED_

#include "Makros.h"
#include "MotorPort.h"
#include "Direction.h"
#include "Controlled.h"

class DLL_DECLSPEC Motor : public Controlled
{

public:
	Motor();
	virtual ~Motor();
	bool on(Direction dir, int speed);
	bool off();
	bool connect(MotorPort port, Controller* controller);
	int last();
	Direction direction();

protected:
	MotorPort mport;
	int lvalue;
	Direction ldirection;

};
#endif // !defined(EA_D8153474_5248_40bc_A5F0_2E1CC65CC2FA__INCLUDED_)
