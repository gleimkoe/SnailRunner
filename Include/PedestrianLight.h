///////////////////////////////////////////////////////////
//  PedestrianLight.h
//  Implementation of the Class PedestrianLight
//  Created on:      21-Mai-2010 11:53:27
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_6A54BBB4_1A4E_4c41_9871_E710603DE926__INCLUDED_)
#define EA_6A54BBB4_1A4E_4c41_9871_E710603DE926__INCLUDED_

#include "Makros.h"
#include "Lamp.h"
#include "Button.h"
#include "Model.h"

/*! Diese Klasse realisiert eine Fußgängerampel für das CrossRoads-Modell. 
	Eine Fußgängerampel besteht aus zwei, einer roten und einer grünen, LED. 
	Zusätzlich hat die Fußgängerampel einen Taster. */

class DLL_DECLSPEC PedestrianLight : public Model
{

public:
	/*! Der Standardkonstruktor mit Angabe der Schnittstelle der roten und
	    grünen LED. */
	PedestrianLight(OutputPort pred=O1,OutputPort pgreen=O2);
	/*! Der Destruktor. */
	virtual ~PedestrianLight();
	/*! Diese Methode verbindet das Objekt mit dem CrossRoads-Modell. */
	virtual bool construct(Controller* controller);
	/*! Diese Methode schaltet die rote LED an. */
	void redOn();
	/*! Diese Methode schaltet die rote LED aus. */
	void redOff();
	/*! Diese Methode schaltet die grüne LED an. */
	void greenOn();
	/*! Diese Methode schaltet die grüne LED aus. */
	void greenOff();
	/*! Diese Methode liefert liefert die grüne LED als Objekt zurück. */
	Lamp& green();
	/*! Diese Methode liefert liefert die grüne LED als Objekt zurück. */
	Lamp& red();
	/*! Diese Methode liefert den Taster als Objekt zurück. */
	Button& button();
	/*! Mit dieser Methode lassen sich die Schnittstellen der beiden LED setzen. */
	void port(OutputPort pred, OutputPort pgreen);
	
private:
	Lamp redlight;
	Lamp greenlight;
	Button pbutton;
};
#endif // !defined(EA_6A54BBB4_1A4E_4c41_9871_E710603DE926__INCLUDED_)
