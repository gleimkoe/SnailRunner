///////////////////////////////////////////////////////////
//  Contact.h
//  Implementation of the Class Contact
//  Created on:      21-Mai-2010 11:53:20
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_A18A02D2_19B5_4668_95DF_6DA58696DCB8__INCLUDED_)
#define EA_A18A02D2_19B5_4668_95DF_6DA58696DCB8__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

/*! Diese Klasse realisiert einen magnetischen Kontaktsensor, wie dieser im
 *  im CrossRoads-Modell verwendet wird (Gullideckel). */

class DLL_DECLSPEC Contact : public Sensor
{

public:
	/*! Der Standardkonstruktor. */
	Contact(std::string text = "Contact");
	/*! Der Destruktor. */
	virtual ~Contact();
	/*! Diese Methode liefert true zurück, wenn seit der letzten Statusabfrage der
	 *  magnetische Kontakt geschlossen worden ist. Wichtig: diese Methode liefert
	 *  nur bei Zustandsänderungen (Flanke) true zurück ansonsten false. */
	bool isContacted();

};
#endif // !defined(EA_A18A02D2_19B5_4668_95DF_6DA58696DCB8__INCLUDED_)
