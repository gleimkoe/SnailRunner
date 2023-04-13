///////////////////////////////////////////////////////////
//  Actor.h
//  Implementation of the Class Actor
//  Created on:      21-Mai-2010 11:53:19
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_5B8C10A5_57EF_44d7_BBFC_E6C21ECBD867__INCLUDED_)
#define EA_5B8C10A5_57EF_44d7_BBFC_E6C21ECBD867__INCLUDED_

#include "Makros.h"
#include "OutputPort.h"
#include "Controlled.h"

/*! Die Actor-Klasse realisiert eine Oberklasse für Fischertechnik-Aktoren. Zzt. werden
 *  nur LED (Lamp-Klasse) unterstützt. Für Motoren existieren eigene Oberklassen. Für den
 *  ROBO IF Controller ist das die Klasse Motor; der TX Controller unterstützt auch die
 *  EncoderMotor-Klasse. */

class DLL_DECLSPEC Actor : public Controlled
{

public:
	/*! Der Konstruktor.
	 * \param po Gibt den Ausgang (Port) an. */
	Actor(OutputPort po=O1);
	/*! Der Destruktor. */
	virtual ~Actor();
	/*! Diese Methode verbindet den Aktor mit einem Ausgang am Controller.
	 * \param port Gibt den Ausgang (Port) an.
	 * \param controller Gibt den Controller an, mit dem der Aktor verbunden ist. */
	bool connect(OutputPort port, Controller* controller);
	/*! Mit dieser Methode lässt sich der Ausgang ändern. */
	void port(OutputPort po);
	/*! Diese Methode setzt den Wert am Ausgang. */
	bool value(int val);
	/*! Diese Methode liefert den zuletzt gesetzten Wert zurück. */
	int last();

private:
	OutputPort oport;
	int lvalue;

};
#endif // !defined(EA_5B8C10A5_57EF_44d7_BBFC_E6C21ECBD867__INCLUDED_)
