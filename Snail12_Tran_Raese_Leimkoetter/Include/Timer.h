///////////////////////////////////////////////////////////
//  Timer.h
//  Implementation of the Class Timer
//  Created on:      21-Mai-2010 11:53:29
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_A919ABDD_0233_432b_BC3C_C7182FA99295__INCLUDED_)
#define EA_A919ABDD_0233_432b_BC3C_C7182FA99295__INCLUDED_

#include "Makros.h"
#include "ControlLoop.h"
#include "FishermanBase.h"

class DLL_DECLSPEC Timer : public FishermanBase
{

public:
	Timer();
	virtual ~Timer();

	void trigger(ControlLoop* callback);
	virtual bool start(int nMilliseconds, bool periodic = true) =0;
	virtual void cancel() =0;

protected:
	ControlLoop *m_ControlLoop;

	bool alarm();

};
#endif // !defined(EA_A919ABDD_0233_432b_BC3C_C7182FA99295__INCLUDED_)
