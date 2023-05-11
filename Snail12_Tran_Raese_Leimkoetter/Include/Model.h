///////////////////////////////////////////////////////////
//  Model.h
//  Implementation of the Class Model
//  Created on:      21-Mai-2010 11:53:26
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_947A2B8C_11FA_4ef2_8FEE_614DA865B245__INCLUDED_)
#define EA_947A2B8C_11FA_4ef2_8FEE_614DA865B245__INCLUDED_

#include "Makros.h"
#include "Controller.h"
#include "FishermanBase.h"

class DLL_DECLSPEC Model : public FishermanBase
{

public:
	Model();
	virtual ~Model();

	virtual bool construct(Controller* controller);
	virtual bool destruct();

//private:
	Controller *m_Controller;

};
#endif // !defined(EA_947A2B8C_11FA_4ef2_8FEE_614DA865B245__INCLUDED_)
