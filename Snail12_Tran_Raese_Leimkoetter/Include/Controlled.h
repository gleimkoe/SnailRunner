///////////////////////////////////////////////////////////
//  Controlled.h
//  Implementation of the Class Controlled
//  Created on:      21-Mai-2010 11:53:20
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_4FFBE329_08DB_4b8e_9C42_7A6F318CBF65__INCLUDED_)
#define EA_4FFBE329_08DB_4b8e_9C42_7A6F318CBF65__INCLUDED_

#include "Makros.h"
#include "FishermanBase.h"
#include "Controller.h"

class DLL_DECLSPEC Controlled : public FishermanBase
{

public:
	Controlled();
	virtual ~Controlled();

	bool connect(Controller* control);

protected:
	Controller *m_Controller;

};
#endif // !defined(EA_4FFBE329_08DB_4b8e_9C42_7A6F318CBF65__INCLUDED_)
