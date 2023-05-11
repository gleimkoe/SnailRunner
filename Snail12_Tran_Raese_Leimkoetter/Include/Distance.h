///////////////////////////////////////////////////////////
//  Distance.h
//  Implementation of the Class Distance
//  Created on:      21-Mai-2010 11:53:23
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_F9710617_8A43_4769_9D6D_53E056A956A9__INCLUDED_)
#define EA_F9710617_8A43_4769_9D6D_53E056A956A9__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

/*! Diese Klasse realisiert einen Ultraschallsensor zur Messung von Entfernungen. */

class DLL_DECLSPEC Distance : public Sensor
{

public:
	/*! Der Standardkonstruktor. */
	Distance(std::string text = "Distance");
	/*! Der Destruktor. */
	virtual ~Distance();

};
#endif // !defined(EA_F9710617_8A43_4769_9D6D_53E056A956A9__INCLUDED_)
