///////////////////////////////////////////////////////////
//  Application.h
//  Implementation of the Class Application
//  Created on:      21-Mai-2010 11:53:19
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_B352BCEC_31B9_4e8d_9423_398A8E48CD77__INCLUDED_)
#define EA_B352BCEC_31B9_4e8d_9423_398A8E48CD77__INCLUDED_

#include "Makros.h"
#include "Model.h"
#include "Controller.h"
#include "ControlLoop.h"
#include "Win32Timer.h"

class DLL_DECLSPEC Application : public FishermanBase
{

public:
	Application(ControlLoop* l);
	virtual ~Application();

	void execute(int nMilliseconds =50);
	void terminate();

private:

	ControlLoop* function;
	Win32Timer* timer;
};
#endif // !defined(EA_B352BCEC_31B9_4e8d_9423_398A8E48CD77__INCLUDED_)
