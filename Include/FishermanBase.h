///////////////////////////////////////////////////////////
//  FishermanBase.h
//  Implementation of the Class FishermanBase
//  Created on:      21-Mai-2010 11:53:25
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_FCF2DEAF_C586_4d1f_8923_5ADA6B7D2C29__INCLUDED_)
#define EA_FCF2DEAF_C586_4d1f_8923_5ADA6B7D2C29__INCLUDED_

#include "Makros.h"

class DLL_DECLSPEC FishermanBase
{

public:
	FishermanBase();
	virtual ~FishermanBase();

	const int errorCode();

protected:
	void errorCode(int error);

private:
	int ecode;

	// --Avoid Copying and Assignment.
	FishermanBase(const FishermanBase&) {}
	FishermanBase& operator=(const FishermanBase&) { return *this; }

};
#endif // !defined(EA_FCF2DEAF_C586_4d1f_8923_5ADA6B7D2C29__INCLUDED_)
