#ifndef _TXCONTROLLERSUPERVISION_H
#define _TXCONTROLLERSUPERVISION_H

#pragma warning( disable: 4251 )

#include <string>

#include "Makros.h"
#include "TxController.h"
#include "TxControllerLogging.h"

/*! Die Klasse TxControllerSupervision realisiert die Ansteuerung eines TX Controllers
 *  von Fischertechnik mit einem reaktiven Modell. */

class DLL_DECLSPEC TxControllerSupervision : public TxController {
public:

	/*! Der Standardkonstruktor. */
	TxControllerSupervision();
	/*! Der Destruktor. */
	~TxControllerSupervision();
	
	/*! Diese Methode liest einen Sensorwert am TX Controller aus.
	 *  \param iPort Gibt den Eingabeport I1,...,I8 an.
	 *  \param nValue Enthält den Sensorwert am gewählten Eingang.
	 *  \param iType Gibt den Typ des Eingangs an.
	 *  HINWEIS: Diese Methode sollte nicht direkt aufgerufen werden. Für das Auslesen
	 *  eines Sensors sollte eine Sensorklasse (Photo, Distance, ...) verwendet werden. */ 
	bool input(InputPort iPort, int& nValue, UniversalInput iType = ANALOG_5KOMEGA);

	/*! Diese Methode setzt einen Zähler zurück.
	 *  \param cPort gibt den Zähler C1,...,C4 an.
	 *  HINWEIS: Diese Methode sollte nicht direkt aufgerufen werden. Für das Zurücksetzen
	 *  eines Zählers sollte die Zählerklasse verwendet werden. */ 
	bool clear(InputPort cPort);

	/*! Diese Methode startet einen Motor im nicht-intelligenten Modus. In diesem
	 *  Modus stoppt der Motor nicht automatisch. 
	 *  \param oport Gibt den Motor M1,...,M4 an.
	 *  \param dir Gibt die Drehrichtung des Motors an.
	 *  \param speed Gibt die Drehgeschwindigkeit 0,...,512 an.
	 *  HINWEIS: Diese Methode sollte nicht direkt aufgerufen werden. Für die Ansteuerung
	 *  des Motors sollte die Motorklasse verwendet werden. */ 
	bool output(MotorPort oport, Direction dir, int speed);

	/*! Diese Methode startet einen Motor im intelligenten Modus. Der Motor stoppt nach einer
	 *  vorgegebenen Anzahl von Drehimpulsen. Es ist möglich, den Motor mit einem zweiten Motor
	 *  zu synchronisieren.
	 *  \param mport Gibt den Motor M1,...,M4 an.
	 *  \param dir Gibt die Drehrichtung des Motors an.
	 *  \param speed Gibt die Geschwindigkeit 0,...,512 an.
	 *  \param impulse Gibt die Drehimpulse an, wann der Motor automatisch gestoppt werden soll.
	 *  \param mport2 Gibt des zweiten Motors zur Synchronisierung an.
	 *  \param dir2 Gibt die Drehrichtung des zweiten Motors an.
	 *  HINWEIS: Diese Methode sollte nicht direkt aufgerufen werden. Für die Ansteuerung
	 *  des Motors sollte die Motorklasse verwendet werden. */ 
	bool output(MotorPort mport, Direction dir, int speed, int impulse, MotorPort mport2, Direction dir2);

	/*! Diese Methode stoppt einen Motor im intelligenten Modus.
	 *  \param mport Gibt den Motor M1,...,M4 an.
	 *  HINWEIS: Diese Methode sollte nicht direkt aufgerufen werden. Für die Ansteuerung
	 *  des Motors sollte die Motorklasse verwendet werden. */ 
	bool stop(MotorPort mport);

	/*! Diese Methode setzt den PWM-Wert (Duty) an einem Ausgang.
	 *  \param oport Gibt den Ausgang O1,...,O8 an.
	 *  \param value Gibt den PWM-Wert 0,...,512 an.
	 *  HINWEIS: Diese Methode sollte nicht direkt aufgerufen werden. Für die Ansteuerung
	 *  eines Aktors sollte eine Aktor-Klasse (z.B. Lamp), aber keine Motorklasse verwendet werden. */ 
	bool output(OutputPort oport, int value);
	
	/*! Diese Methoden konfigurieren die Eingänge und die Ausgänge des TX Controllers.
	 *  HINWEIS: Diese Methoden werden durch die Sensor- und Aktorklassen aufgerufen. */
	bool configure(InputPort iPort, UniversalInput iType = ANALOG_5KOMEGA, std::string name = "unknown");
	bool configure(MotorPort);
	bool configure(OutputPort oport);
	
	/*! Diese Methode ermöglicht zur Fehlersuche, Zeichenketten in die Ausgabedatei (siehe
	    StartSupervision-Funktion) zeitlich sychronisiert zu schreiben. 
		\param note Gibt die Zeichenkette zur Ausgabe an. */
	void annotate(const std::string& note);
	
	/*! --MotorMode wird zzt. nicht unterstützt. */
	enum MotorMode { 
		EXECUTE_RUNNING_COMMAND, /*! Neuer Befehl zur Motorsteuerung wird unterdrückt. */
		ABORT_RUNNING_COMMAND	 /*! Laufender Befehl zur Motorsteuerung wird abgebrochen. */
	};

	/*! Diese Methode wird in der aktuellen Version nicht unterstützt. */
	MotorMode motorMode() const;
	/*! Diese Methode wird in der aktuellen Version nicht unterstützt. */
	void motorMode(MotorMode nmode);

	/*! Diese Methode ist nur für internes Logging notwendig. */
	const TxControllerLogging& logconf() const { return _logging; }

private:

	/* --Motormode. */
	MotorMode _motorMode;

	/* --For logging purpose. */
	TxControllerLogging _logging;

};


#endif