///////////////////////////////////////////////////////////
//  Controller.h
//  Implementation of the Class Controller
//  Created on:      21-Mai-2010 11:53:21
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_46AC17A9_E115_4d27_A1F6_F59EE1C9A235__INCLUDED_)
#define EA_46AC17A9_E115_4d27_A1F6_F59EE1C9A235__INCLUDED_

#include "Makros.h"
#include "UniversalInput.h"
#include "InputPort.h"
#include "Direction.h"
#include "MotorPort.h"
#include "OutputPort.h"
#include "FishermanBase.h"

#include <iostream>
#include <string>
using namespace std;

/*! Die Controller-Klasse ist die Oberklasse für alle weiteren Controller, die zzt.
 *  unterstützt werden: RbController inkl. CrossRoads, TxController und EmuController.*/

class DLL_DECLSPEC Controller : public FishermanBase
{

public:
	/*! Der Standardkonstruktor. */
	Controller();
	/*! Der Destruktor. */
	virtual ~Controller();

	virtual bool input(InputPort iPort, int& nValue, UniversalInput iType = ANALOG_5KOMEGA) =0;

	/* --Standard Mode: enabling/disabling. */
	virtual bool output(MotorPort oport, Direction dir, int speed) =0;

	virtual bool output(OutputPort oport, int value) =0;
	virtual bool clear(InputPort cPort) =0;
	virtual bool configure(InputPort iPort, UniversalInput iType = ANALOG_5KOMEGA, std::string info = "unknown") =0;
	virtual bool configure(MotorPort) =0;
	virtual bool configure(OutputPort oport) =0;


	/*! Die Methode liefert die Hauptversion der verwendeten DLL. */
	virtual unsigned int major() const =0; 
	/*! Die Methode liefert die untergeordnete Version der verwendeten DLL. */
	virtual unsigned int minor() const =0; 
	/*! Die Methode liefert die Nebenversion der verwendeten DLL. */
	virtual unsigned int sub() const =0;
	/*! Die Methode liefert die Seriennummer des Controllers zurück. */
	virtual const char* serialnumber() const =0; 
	/*! Die Methide liefert die Firmware-Kennung des Controllers zurück. */
	virtual const char* firmware() const =0; 
	/*! Die Methode liefert den Namen des Herstellers des Controllers zurück. */
	virtual const char* manufacturer() const =0;
	/*! Die Methode liefert den Namen des Gerätes zurück. */
	virtual const char* name() const =0; 
	/*! Die Methode liefert den Kurznamen des Gerätes zurück. */
	virtual const char* shortname() const =0;

	/*! Diese Methode gibt einige Informationen zu der Fischertechnik-Software in den
	 *  Ausgabestrom. */
	virtual void banner(ostream& out =cout) const;

private:
	Controller *m_Controller;

};
#endif // !defined(EA_46AC17A9_E115_4d27_A1F6_F59EE1C9A235__INCLUDED_)
