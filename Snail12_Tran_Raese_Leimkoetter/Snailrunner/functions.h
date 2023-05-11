/*
Funktion 1: Kalibrierung
Die Kalibrierung soll durch eine eigene Menüstruktur umgesetzt werden und folgende Anforderungen erfüllen:

• Anzeige der aktuellen Schwellwerte der Farb- und Ultraschallsensoren - DONE
• Manuelle Änderung der Schwellwerte. - DONE
  Die vom Benutzer eingegebenen Werte sollen hinsichtlich gültiger Wertebereiche überprüft werden. - DONE
• Automatische Kalibrierung der Schwellwerte der Farbbereiche durch Ermittlung der aktuellen
  Farbwerte. Hierbei ist folgender Ablauf erforderlich: - TODO

    o Der Roboter wird auf eine bestimmte Farbe gestellt - DONE LOL
    o Der Roboter fährt eine kurze Strecke (ca. 10 bis 20 cm) - DONE
    o Während der Fahrt werden die Farbwerte ermittelt und der Mittelwert bestimmt - TODO: Array/Vector anlegen, mit for schleife durchlaufen und Werte speichern
    o Sind alle Farbwerte durch eine Messung bestimmt worden, werden die Schwellwerte - TODO: siehe Anleitung
      automatisch in sinnvoller Weise berechnet.

• Speicherfunktion, d.h. die Schwellwerte werden in einer separaten Datei abgespeichert.
  Das Speichern kann wahlweise manuell und/oder automatisch (z.B. nach jeder Änderung) erfolgen. - DONE
• Ladefunktion, d.h. beim Programmstart wird geprüft, ob die Datei mit den Schwellwerten vorhanden ist, - DONE
  die Datei wird ausgelesen und die Schwellwerte im Programm übernommen. - DONE
  Ist keine Datei vorhanden, werden Standardwerte für die Schwellwerte verwendet. - DONE

  Menueeintrag in main.cpp vom Snailrunner hinzuguegen - TODO


Funktion 2: Einstellungen
Der Benutzer soll einen neuen Menüpunkt Einstellungen für folgende Fahrauftragsdaten erhalten:
    • Startposition (Startläufer, Weiterläufer),
    • Fahrtrichtung (gegen den Uhrzeigersinn/im Uhrzeigersinn),
    • Anzahl der zu fahrenden Runden (1 bis 2)

Mögliche fehlerhafte Eingaben sollen vom Programm verhindert werden.


Funktion 3: Staffellauf ohne Hindernisse



Funktion 4: Logging
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "SnailRunner.h"
#include <ctime>

void calibrationMenu(SnailRunner* robot);
void settingsMenu(SnailRunner* robot);
void logStartConditions(SnailRunner* robot);
void logLapBanner(SnailRunner* robot);
void logOFF_TRAIL(SnailRunner* robot);
void logCorner(SnailRunner* robot);
void logLapConclusion(SnailRunner* robot);

#endif