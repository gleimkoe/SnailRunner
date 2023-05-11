///////////////////////////////////////////////////////////
//  Sensor.h
//  Implementation of the Class Sensor
//  Created on:      21-Mai-2010 11:53:28
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_44D7C22D_F4D2_47ee_ACC1_F7138B7BF8A8__INCLUDED_)
#define EA_44D7C22D_F4D2_47ee_ACC1_F7138B7BF8A8__INCLUDED_

#pragma warning( disable: 4251 )

#include <string>

#include "Makros.h"
#include "InputPort.h"
#include "UniversalInput.h"
#include "Controlled.h"

/*! Diese Klasse ist die Oberklasse für alle Fischertechnik-Sensoren. */

class DLL_DECLSPEC Sensor : public Controlled
{

public:
	/*! Der Standardkonstruktor. 
	    \param utype gibt den Typ der Schnittstelle (Eingang) an.
		\param ip gibt die Nummer des Eingangs an. */
	Sensor(UniversalInput utype =ANALOG_5KOMEGA,InputPort ip=I1,std::string text="Sensor");
	/*! Der Destruktor. */
	virtual ~Sensor();
	/*! Diese Methode liefert den Sensorwert zurück. Die Einheit des Wertes
	 *  ist abhängig vom Sensortyp. */
	virtual int value();
	/*! Diese Methode verbindet den Sensor mit einem Eingang. */
	bool connect(InputPort port, Controller* controller);
	/*! Diese Methode setzt den Eingang. */
	void port(InputPort ip);
	/*! Diese Methode liefert den Eingang zurück. */
	InputPort port() const { return iport; }

protected:
	InputPort iport;
	UniversalInput uType;
	std::string textual;
};
#endif // !defined(EA_44D7C22D_F4D2_47ee_ACC1_F7138B7BF8A8__INCLUDED_)
