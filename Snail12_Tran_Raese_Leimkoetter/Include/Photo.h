///////////////////////////////////////////////////////////
//  Photo.h
//  Implementation of the Class Photo
//  Created on:      21-Mai-2010 11:53:27
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_96B13CE6_2C21_40cb_BA4C_4E8482348508__INCLUDED_)
#define EA_96B13CE6_2C21_40cb_BA4C_4E8482348508__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

/*! Die Photo-Klasse implementiert einen Helligkeitssensor, wie dieser beispielsweise
 *  im CrossRoads-Modell zu finden ist (links oben). Die Klasse stellt eine Methode
 *  isDark() bereit, mit der sich überprüfen lässt, ob es dunkel ist.*/

class DLL_DECLSPEC Photo : public Sensor
{

public:
	/*! Der Standardkonstruktor. */
	Photo(std::string text = "Photo");
	/*! Der Destruktor. */
	virtual ~Photo();
	/*! Die Methode liefert true zurück, wenn es dunkel ist. Hierzu wird ein zuvor
	 *  definierter Schwellenwert überprüft. */
	bool isDark();
	/*! Diese Methode liefert der Schwellenwert zurück. */
	int darkness();
	/*! Diese Methode setzt den Schwellenwert zur Unterscheidung hell/dunkel. Je kleiner
	 *  der Wert, umso dunkler muss es zur Unterscheidung sein. Der Standardwert ist 0 
	 *  (stockfinster). */
	void darkness(int ndark);

private:
	int darkvalue;

};
#endif // !defined(EA_96B13CE6_2C21_40cb_BA4C_4E8482348508__INCLUDED_)
