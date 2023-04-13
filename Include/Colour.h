#ifndef COLOUR_H_
#define COLOUR_H_

#include "Makros.h"
#include "Sensor.h"

/*! Die Klasse Colour implementiert den Fischertechnik-Farbsensor. Der Farbsensor
 *  liefert ein analoges Signal. */

class DLL_DECLSPEC Colour : public Sensor
{
public:
	/*! Der Standardkonstruktor. */
	Colour(std::string text="Colour");
	/*! Der Destruktor. */
	virtual ~Colour();
};

#endif