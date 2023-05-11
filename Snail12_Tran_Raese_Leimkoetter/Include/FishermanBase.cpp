///////////////////////////////////////////////////////////
//  FishermanBase.cpp
//  Implementation of the Class FishermanBase
//  Created on:      17-Jan-2010 10:12:10
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "FishermanBase.h"

FishermanBase::FishermanBase():ecode(0){}

FishermanBase::~FishermanBase(){}

const int FishermanBase::errorCode(){
	return ecode;
}


void FishermanBase::errorCode(int error){
	ecode=error;
}
