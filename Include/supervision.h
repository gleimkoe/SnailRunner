#ifndef _SUPERVISION_H
#define _SUPERVISION_H

#include "Makros.h"
#include "ReactiveModel.h"

/*! Dieser Aufzählungstyp definiert, wie das Fischertechnik-Modell ausgeführt werden soll. 
 *  Durch Auswahl NON_REACTIVE werden die on_XYZ_Changed-Methoden nicht automatisch aufgerufen.
 *  Es können aber alle Sensorwerte abgefragt und auch die Motoren angesteuert werden. Durch
 *  Auswahl von REACTIVE wird die onStart-Methode des Modells ausgeführt und im folgenden bei
 *  Änderungen an den Eingängen/Motoren die zugehörigen on_XYZ_Changed-Methoden. Wichtiger
 *  Hinweis: Ist das Modell im reaktiven Modus, dann sollte die Steuerung des Modells nur durch
 *  die on_XYZ_Changed-Methoden erfolgen! */
enum ReactiveModelMode {
	NON_REACTIVE,	/*! Einfacher Modus. */ 
	REACTIVE		/*! Reaktiver Modus. */
};

/*! Dieser Aufzählungstyp wird zzt. nicht unterstützt. */
enum ReactiveModelPlayback {NO_PLAYBACK, PLAYBACK_ONCE, PLAYBACK_LOOP} ;

/*! Diese Funktion startet die Ausführung eines Fischertechnikmodells. Erst nach Aufruf dieser
 *  Funktion wird das Modell aktiv. Alle Änderungen der Sensorwerte und Motorenzustände werden
 *  mit einer hohen zeitlichen Auflösung in einer Datei geschrieben. Der Inhalt dieser Datei kann
 *  zur Fehlersuche eingesetzt werden. Die Datei wird mit jedem Aufruf der StartSupervision-Funktion
 *  überschrieben. 
 *	\param model Gibt das Fischertechnikmodell an.
 *	\param mode Gibt den Modus der Ausführung an.
 *	\param ofile Gibt den Dateinamen zur Ausgabe von Sensorwerten etc. zu Debugzwecken an
 *	\param pback Wird zzt. nicht unterstützt.
 *	\param ifile Wird zzt. nicht unterstützt. */
DLL_DECLSPEC void StartSupervision(ReactiveModel* model, ReactiveModelMode mode = REACTIVE, const char* ofile = "txcontroller.txt",
		ReactiveModelPlayback pback =NO_PLAYBACK, const char* ifile =0);

/*! Mit dieser Funktion kann die Ausführung eines Fischertechnik-Modells beendet werden. */
DLL_DECLSPEC void StopSupervision();

/*! Interne Funktionen. */
DLL_DECLSPEC int GetUniversalInput(InputPort port);
DLL_DECLSPEC int GetCounter(InputPort port);
DLL_DECLSPEC int GetCounterConfiguration(InputPort port);
DLL_DECLSPEC void SetAnnotationSupervision(const std::string& note);

#endif
