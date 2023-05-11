///////////////////////////////////////////////////////////
//  Button.h
//  Implementation of the Class Button
//  Created on:      21-Mai-2010 11:53:20
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_70572C16_296A_4f36_9F0C_55C81E78DC74__INCLUDED_)
#define EA_70572C16_296A_4f36_9F0C_55C81E78DC74__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

/*! Die Klasse Button realisiert einen Taster, beispielsweise den rot-schwarzen 
 *  Fischertechnik-Baustein oder auch den Berührungstaster im CrossRoads-Modell. */

class DLL_DECLSPEC Button : public Sensor
{

public:
	/*! Der Standardkonstruktor. */
	Button(std::string text = "Button");
	/*! Der Destruktor. */
	virtual ~Button();
	/*! Diese Methode liefert true zurück, wenn der Taster seit der letzten Abfrage
	 *  gedrückt worden ist. Wichtig: diese Methode reagiert nur auf Veränderungen des 
	 *  Zustands! Um festzustellen, ob der Taster gedrückt ist, unabhängig von Zustandsänderungen,
	 *  so kann dieses über den Aufruf der Methode value erfolgen. */
	bool isPressed();
	/*! Diese Methode liefert true zurück, wenn der Taster seit der letzten Abfrage
	 *  losgelassen worden ist. Wichtig: Es werden nur Zustandsänderungen signalisiert (siehe
	 *  isPressed-Methode. */
	bool isReleased();
};
#endif // !defined(EA_70572C16_296A_4f36_9F0C_55C81E78DC74__INCLUDED_)
