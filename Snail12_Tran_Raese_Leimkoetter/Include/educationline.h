#ifndef EDUCATIONLINE_H
#define EDUCATIONLINE_H

#include "Makros.h"
#include "OutputPort.h"
#include "InputPort.h"

#include <vector>

class DLL_DECLSPEC EducationLine {
public:

	enum class LED {
		/* --Verkehrsampeln. */
		/* --Hauptstraße rechts.*/
		MAIN_ROAD_RIGHT_RED = O4,
		MAIN_ROAD_RIGHT_YELLOW = O5,
		MAIN_ROAD_RIGHT_GREEN = O6,
		/* --Hauptstraße links. */
		MAIN_ROAD_LEFT_RED = O1,
		MAIN_ROAD_LEFT_YELLOW = O2,
		MAIN_ROAD_LEFT_GREEN = O3,
		/* --Seitenstraße oben. */
		SIDE_ROAD_TOP_RED = O12,
		SIDE_ROAD_TOP_YELLOW = O13,
		SIDE_ROAD_TOP_GREEN = O14,
		/* --Seitenstraße unten. */
		SIDE_ROAD_BOTTOM_RED = O9,
		SIDE_ROAD_BOTTOM_YELLOW = O10,
		SIDE_ROAD_BOTTOM_GREEN = O11,

		/* --Fussgängerampeln. */
		/* --Hauptstraße rechts, oben. */
		MAIN_ROAD_RIGHT_TOP_RED = O19,
		MAIN_ROAD_RIGHT_TOP_GREEN = O20,
		/* --Hauptstraße rechts, unten. */
		MAIN_ROAD_RIGHT_BOTTOM_RED = O27,
		MAIN_ROAD_RIGHT_BOTTOM_GREEN = O28,
		/* --Hauptstraße links, oben. */
		MAIN_ROAD_LEFT_TOP_RED = O25,
		MAIN_ROAD_LEFT_TOP_GREEN = O26,
		/* --Hauptstraße links, unten. */
		MAIN_ROAD_LEFT_BOTTOM_RED = O17,
		MAIN_ROAD_LEFT_BOTTOM_GREEN = O18,
		/* --Seitenstraße oben, rechts. */
		SIDE_ROAD_TOP_RIGHT_RED = O23,
		SIDE_ROAD_TOP_RIGHT_GREEN = O24,
		/* --Seitenstraße oben, links. */
		SIDE_ROAD_TOP_LEFT_RED = O31,
		SIDE_ROAD_TOP_LEFT_GREEN = O32,
		/* --Seitenstraße unten, rechts. */
		SIDE_ROAD_BOTTOM_RIGHT_RED = O21,
		SIDE_ROAD_BOTTOM_RIGHT_GREEN = O22,
		/* --Seitenstraße unten, links. */
		SIDE_ROAD_BOTTOM_LEFT_RED = O29,
		SIDE_ROAD_BOTTOM_LEFT_GREEN = O30,

		/* --Blitzer. */
		FLASH = O15,
		/* --Straßenbeleuchtung*/
		LIGHTING = O16

	};

	enum class Interaction {
		/* --Taster für Fußgängerüberweg. */
		/* --Hauptstraße rechts. */
		MAIN_ROAD_RIGHT_TOP = I3,
		MAIN_ROAD_RIGHT_BOTTOM = I4,
		/* --Hauptstraße links. */
		MAIN_ROAD_LEFT_TOP = I2,
		MAIN_ROAD_LEFT_BOTTOM = I1,
		/* --Seitenstraße oben. */
		SIDE_ROAD_TOP_RIGHT = I8,
		SIDE_ROAD_TOP_LEFT = I7,
		/* --Seitenstraße unten. */
		SIDE_ROAD_BOTTOM_RIGHT = I5,
		SIDE_ROAD_BOTTOM_LEFT = I6,
		
		/* --"Gullideckel" in den Straßen. */
		MAIN_ROAD_RIGHT_CAR = I10,
		MAIN_ROAD_LEFT_CAR = I9,
		SIDE_ROAD_TOP_CAR = I12,
		SIDE_ROAD_BOTTOM_CAR = I11,
		CENTER_LEFT_CAR = I14,
		CENTER_RIGHT_CAR = I13
	};

	/* --Hilfsklasse, die verschiedenen Ein- und Ausgänge für einen der vier Übergänge zusammenfasst. */
	struct DLL_DECLSPEC CrosswalkDefinition {
		/* --Verkehrsampel. */
		EducationLine::LED red,yellow,green;
		EducationLine::Interaction car;
		/* --Fussgängerampel. */
		EducationLine::LED rightRed, rightGreen;
		EducationLine::LED leftRed, leftGreen;
		EducationLine::Interaction right, left;
		
		CrosswalkDefinition(EducationLine::LED r, EducationLine::LED y, EducationLine::LED g,
			EducationLine::Interaction c, EducationLine::LED rr, EducationLine::LED rg,
			EducationLine::LED lr, EducationLine::LED lg,
			EducationLine::Interaction rbu, EducationLine::Interaction lbu) :
			red(r), yellow(y), green(g), car(c),
			rightRed(rr), rightGreen(rg),
			leftRed(lr), leftGreen(lg),
			right(rbu), left(lbu) {}
	};

	/* --Hilfsobjekte enthalten Definitionen für jeweils einen Übergang. */
	static const CrosswalkDefinition LeftDefinition;
	static const CrosswalkDefinition RightDefinition;
	static const CrosswalkDefinition TopDefinition;
	static const CrosswalkDefinition BottomDefinition;

	EducationLine& operator=(const EducationLine&) = delete;
	
	using VLED = std::vector<LED>;

	/* --Allgemeine Fehlerklasse. */
	class CrossRoadsFailure {};
	
	/* --Initialisierung. */
	static void initialize();

	/* --Dauerhaftes Einschalten einer LED. */
	static void enable(LED led);

	/* --Dauerhaftes Ausschalten einer LED. */
	static void disable(LED led);

	/* --Helligkeitssensor. */
	static int brightness();

	/* --Ausführung für ms Millisekunden pausieren.*/
	static void sleep(unsigned int ms);

	/* --Liefert true zurück, wenn Taste/Kontakt ist gedrückt ansonsten false. 
	   Hinweis: Es wird nur die Flanke betrachtet, d.h. wird die Taste gedrückt,
	   liefert die Methode einmalig true zurück. */
	static bool isPressed(Interaction button);

	/* --Liefert true zurück, wenn Taste/Kontakt losgelassen ist ansonsten false (Hinweis s.o.). */
	static bool isReleased(Interaction button);

	/* --Blockiert, bis eine Taste/Kontakt gedrückt und wieder losgelassen worden ist.*/
	static Interaction input();

	/* --Testfunktion (intern). */
	static void test();
	static void test2();

};

#endif
