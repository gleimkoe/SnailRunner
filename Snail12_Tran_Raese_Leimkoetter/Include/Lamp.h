///////////////////////////////////////////////////////////
//  Lamp.h
//  Implementation of the Class Lamp
//  Created on:      21-Mai-2010 11:53:25
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_08C5E1DD_6B6C_424a_AA34_E689A5A52CF9__INCLUDED_)
#define EA_08C5E1DD_6B6C_424a_AA34_E689A5A52CF9__INCLUDED_

#include "Makros.h"
#include "Actor.h"

/*! Diese Klasse realisiert eine LED-Lampe. */

class DLL_DECLSPEC Lamp : public Actor
{

public:
	/*! Der Standardkonstruktor. */
	Lamp(OutputPort po=O1);
	/*! Der Destruktor. */
	virtual ~Lamp();
	/*! Mit dieser Methode wird die Lampe eingeschaltet. */
	bool on();
	/*! Mit dieser Methode wird die Lampe ausgeschaltet. */
	bool off();
	/*! Diese Methode liefert true zurück, wenn die Lampe ausgeschaltet ist. */
	bool isOff();
	/*! Diese Methode liefert true zurück, wenn die Lampe eingeschaltet ist. */
	bool isOn();
	/*! Mit dieser Methode kann zwischen an und aus hin- und hergeschaltet werden. */
	bool toggle();
};
#endif // !defined(EA_08C5E1DD_6B6C_424a_AA34_E689A5A52CF9__INCLUDED_)
