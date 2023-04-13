///////////////////////////////////////////////////////////
//  Model.cpp
//  Implementation of the Class Model
//  Created on:      09-Jan-2010 21:39:04
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

// --Including the Windows stuff.
#include <Windows.h>

#include "Model.h"

Model::Model(){

}



Model::~Model(){

}


bool Model::construct(Controller* controller){
	/* --Keep pointer to the controller object. */
	m_Controller=controller;
	return true;
}


bool Model::destruct(){
	bool retvalue=false;
#if 0
	/* --Reset the controller object. */
	if (m_Controller) {
		retvalue=m_Controller->finalize();
		/* --Delete the link to the controller. */
		m_Controller=0;
	}
#endif
	return retvalue;
}
