///////////////////////////////////////////////////////////
//  ControlLoop.h
//  Implementation of the Class ControlLoop
//  Created on:      21-Mai-2010 11:53:22
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_5B8477E2_AA81_4bf9_A613_0FAF1AD46E9F__INCLUDED_)
#define EA_5B8477E2_AA81_4bf9_A613_0FAF1AD46E9F__INCLUDED_

#include "Makros.h"
#include "EventField.h"
#include "FishermanBase.h"

/*! Die ControlLoop-Klasse implementiert die abstrakte Schnittstelle der Ablauflogik für ein 
 *  konkretes Fischertechnik-Modell. Bei den Fischertechnik-Modellen handelt es sich um ein
 *  reaktives System, d.h. die tatsächlichen Aktionen werden durch Ereignisse ausgelöst. Ein
 *  Ereignis kann beispielsweise das Drücken eines Tasters oder das Überfahren einer Linie
 *  mit einem Roboter sein.
 *
 *  Die Fischertechnik-Controller arbeiten im Polling-Betrieb, d.h. in periodischen Abständen
 *  müssen die Eingänge abgefragt werden. Hierzu muss die step()-Methode aufgerufen werden.
 *  Dieses geschieht beispielsweise durch einen Windows-Timer oder durch einen Qt-Timer. */

class DLL_DECLSPEC ControlLoop : public FishermanBase
{

public:
	/*! Der Standardkonstruktor. */
	ControlLoop();
	/*! Der Destruktor. */
	virtual ~ControlLoop();
	/*! Diese Methode muss in abgeleiteten, konkreten Klassen implementiert werden. Die check-Methode
	 *  sollte verwendet werden zu überprüfen, ob und welche Ereignisse an dem Modell ausgelöst worden
	 *  sind. Hierzu sind in dieser Methode die Eingänge zu überprüfen.
	 *  \code
     * 	// --Check if motor is stopped
	 *	if (robot->left().isStopped() && robot->right().isStopped()) 
	 *		efield.set(EVENT_IS_STOPPED);
	 *  \endcode
	 *  In diesem Code-Beispiel wird zuerst überprüft, ob der Roboter steht und nicht fährt. Dieses
	 *  erfolgt durch den Aufruf der isStopped-Methode des linken und rechten Motors. Wenn beide
	 *  Motoren gestoppt sind, dann wird das Ereignis EVENT_IS_STOPPED gesetzt.
	 *
	 *  Hinweis: Die act-Methode wird nur dann ausgeführt, wenn mindestens 1 Ereignis gesetzt worden
	 *  ist. Anderenfalls wird die Ausführung der act-Methode übersprungen. */
	virtual bool check() =0;
	/*! Diese Methode realisiert die Aktionen, die in Abhängigkeit von Ereignissen auszuführen sind.
	 *  Dieses sollte anhand einer Zustandsmaschine erfolgen.
	 *  \code
	 *  if (state==LOST) {
	 *		// --LOST -> LOOKLEFT.
	 *		changeState(LOOKLEFT);	
	 *		robot->turnleft(search*TURN_IMPULSE);
	 *		// --Suchradius erhöhen
	 *		changeSearch();
	 *	// --LOSTRIGHT
	 *	} else if (state==LOSTLEFT) {
	 *		// --LOST -> LOOKRIGHT. 
	 *		changeState(LOOKRIGHT);	
	 *		robot->turnright(search*TURN_IMPULSE);
	 *		// --Suchradius erhöhen
	 *		changeSearch();
	 *	} 
     *
	 *
	 *  // --Clear all others flags
	 *	efield.reset();
     *
	 *	return retvalue;
     *
	 *  \endcode
     *
	 *  Hinweis: Die act-Methode wird solange immer wieder ausgeführt, bis alle Ereignisse gelöscht
	 *  sind. Um eine Endlosschleife zu verhindern, sollte das Ereignisfeld vor dem Rücksprung zurück
	 *  gesetzt werden (wie im Beispiel gezeigt). */
	virtual bool act() =0;
	/*! Diese Methode realisiert einen Durchlauf der Kontrollschleife. Hierzu werden dann
	 *  nachfolgend die Methoden check und act ausgeführt. Die step-Methode muss regelmäßig
	 *  durch einen Trigger aktiviert werden. Dieses geschieht üblicherweise durch einen
	 *  Windows oder Qt-Timer. */
	virtual bool step();
	/*! Diese Methode liefert das Ereignisfeld der Kontrollschleife zurück. */
	EventField& events();
	/*! Diese Methode liefert die Anzahl der Aufrufe der step()-Methode zurück. Aufgrund
	 *  einer konstanten Frequenz, beispielsweise 10Hz, kann diese Methode als Zeitgeber
	 *  verwendet werden.
	 *  \code
	 *  // --Inside of the check-method.
	 *	// --Blink flag
	 *	if (calls()%10==0)
	 *		efield.set(EVENT_BLINK);
	 *  \endcode 
	 *  Hierdurch wird jede 1 Sekunde ein Ereignis ausgelöst. In der act-Methode kann dann
	 *  daraufhin reagiert werden. */
	unsigned int calls();
	/*! Diese Methode liefert die vergangenen Zeit seit dem erstmaligen Ausführen der
	 *  step-Methode in ticks (100ns) zurück. Analog zu der calls-Methode kann diese Methode
	 *  als präziser Zeitgeber verwendet werden. */
	unsigned long long ticks();

protected:
	EventField efield;

	/*! Mit dieser Methode wird ein Ereignis in Abhängigkeit eines oder mehrerer booleschen
	 *  Werte (maximal 5 Werte möglich) gesetzt. Diese Werte werden durch ein logisches Und 
	 *  miteinander verknüpft, d.h. wenn alle Werte zu true ausgewertet werden, dann wird auch 
	 *  das Ereignis gesetzt.
	 *  \param ev  Ereignis [0..63]
	 *  \param b0 1. Boolescher Wert
	 *  \param b1 2. Boolescher Wert
	 *  \param b2 3. Boolescher Wert
	 *  \param b3 4. Boolescher Wert
	 *  \param b4 5. Boolescher Wert
	 */
	void bind(int ev,bool b0 =true, bool b1 =true, bool b2 =true, bool b3 =true, bool b4 =true);

private:
	unsigned int ncalls;
	unsigned long long nticks;

	unsigned long long convertTicks();

};
#endif // !defined(EA_5B8477E2_AA81_4bf9_A613_0FAF1AD46E9F__INCLUDED_)
