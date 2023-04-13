///////////////////////////////////////////////////////////
//  Temperature.h
//  Implementation of the Class Temperature
//  Created on:      21-Mai-2010 11:53:28
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_E74E9FFA_BA57_49f5_9456_2C86A8396D32__INCLUDED_)
#define EA_E74E9FFA_BA57_49f5_9456_2C86A8396D32__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

class DLL_DECLSPEC Temperature : public Sensor
{

public:
	Temperature(std::string text = "Temperature");
	virtual ~Temperature();

};
#endif // !defined(EA_E74E9FFA_BA57_49f5_9456_2C86A8396D32__INCLUDED_)
