///////////////////////////////////////////////////////////
//  Trail.h
//  Implementation of the Class Trail
//  Created on:      21-Mai-2010 11:53:29
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_A34CDED7_BAC0_4bfa_B034_6B537C3850DA__INCLUDED_)
#define EA_A34CDED7_BAC0_4bfa_B034_6B537C3850DA__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

/*! Die Trail-Klasse implementiert den Fischertechnik-Spursensor. Der Trail-Sensor
 *  liefert ein digitales Signal in Abhängigkeit, ob der Untergrund hell oder dunkel
 *  ist. Hierdurch ist es sehr einfach möglich, einer hellen Spur auf dunklem Untergrund
 *  oder umgekehrt zu folgen. 
 *
 *  Hinweis: Der Fischertechnik-Spursensor besteht aus 2 Helligkeitsensoren. Sollen beide
 *  Sensoren verwendet werden, dann müssen 2 Objekte der Trail-Klasse instanziiert werden.
 *  Über die connect-Methode wird der entsprechende rechte oder linke Sensor angesprochen. */

class DLL_DECLSPEC Trail : public Sensor
{

public:
	/*! Der Standardkonstruktor. */
	Trail(std::string text = "Trail");
	/*! Der Destruktor. */
	virtual ~Trail();
	/*! Diese Methode liefert true zurück, wenn der Spursensor über der Spur steht. Die
	 *  Spurfarbe wird über die Methoden trackWhite (helle Spur) oder trackBlack (dunkle
	 *  Spur) eingestellt. Standardmäßig wird eine schwarze Spur verfolgt. */
	bool isOnTrack();
	/*! Diese Methode stellt eine helle Spur zum Verfolgen ein. */
	void trackWhite();
	/*! Diese Methode stellt eine dunkle Spur zum Verfolgen ein. */
	void trackBlack();
	/*! Diese Methode liefert true zurück, wenn eine helle Spur verfolgt werden soll. Diese
	 *  bedeutet aber nicht, dass der Sensor sich über einer hellen Spur befindet. Hierzu
	 *  muss die Methode isOnTrack verwendet werden. */
	bool isTrackWhite();

private:
	bool isTrackWhiteValue;

};
#endif // !defined(EA_A34CDED7_BAC0_4bfa_B034_6B537C3850DA__INCLUDED_)
