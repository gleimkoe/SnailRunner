///////////////////////////////////////////////////////////
//  CrossRoads.h
//  Implementation of the Class CrossRoads
//  Created on:      21-Mai-2010 11:53:23
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_31C4818E_B02D_4b90_B7C2_58CB7BEA9BCB__INCLUDED_)
#define EA_31C4818E_B02D_4b90_B7C2_58CB7BEA9BCB__INCLUDED_

#include "CrossWalk.h"
#include "Lamp.h"
#include "Photo.h"
#include "Contact.h"
#include "Model.h"

/*! Diese Klasse (Template) beschreibt die komplette Kreuzung eines CrossRoads-Modells.
 *  Im wesetnlichen besteht die Kreuzung auf 4 Fußgängerüberwegen und zusätzlich aus einer
 *  Straßenbeleuchtung mit Helligkeitssensor, einer zusätzlichen roten LED zur Simulation
 *	eines "Blitzers" und zwei zusätzlichen Kontaktsensoren in der Kreuzungsmitte.
 *  Hinweis: Es existiert eine konkrete CrossRoads-Klasse, die die CrossWalk-Klasse als
 *  Fußgängerüberweg verwendet.
 */

template<class CROSSWALK> class GenericCrossRoads : public Model
{

public:
	/*! Der Standardkonstruktor. */
	GenericCrossRoads();
	/*! Der Destruktor. */
	virtual ~GenericCrossRoads();

	/*! Diese Methode verbindet das Modell mit dem Controller. */
	virtual bool construct(Controller* controller);

	/*! Diese Methode liefert den linken (in der Draufsicht) Fußgängerüberweg. */
	CROSSWALK& left();
	/*! Diese Methode liefert den unteren (in der Draufsicht) Fußgängerüberweg. */
	CROSSWALK& bottom();
	/*! Diese Methode liefert den oberen (in der Draufsicht) Fußgängerüberweg. */
	CROSSWALK& top();
	/*! Diese Methode liefert den rechten (in der Draufsicht) Fußgängerüberweg. */
	CROSSWALK& right();
	/*! Diese Methode liefert den "Blitzer" (LED). */
	Lamp& camera();
	/*! Diese Methide liefert die Straßenbeleuchtung. */
	Lamp& lighting();
	/*! Diese Methode liefert den Helligkeitssensor zurück. */
	Photo& lightness();
	/*! Diese Methode liefert den linken (in der Draufsicht) Kontaktsensor in der Kreuzungsmitte. */
	Contact& contactLeft();
	/*! Diese Methode liefert den rechten (in der Draufsicht) Kontaktsensor in der Kreuzungsmitte. */
	Contact& contactRight();

	void test();
	void test2();
	void test3();

private:
	CROSSWALK rightcrosswalk;
	CROSSWALK topcrosswalk;
	CROSSWALK leftcrosswalk;
	CROSSWALK bottomcrosswalk;
	Lamp roadlighting;
	Lamp speedcamera;
	Photo photolight;
	Contact contactleft;
	Contact contactright;

};

/*! Eine konkrete Klasse, die die CrossWalk-Klasse verwendet. */
typedef GenericCrossRoads<CrossWalk> CrossRoads;

template<class CROSSWALK> 
inline GenericCrossRoads<CROSSWALK>::GenericCrossRoads(){}

template<class CROSSWALK> 
inline GenericCrossRoads<CROSSWALK>::~GenericCrossRoads(){}

template<class CROSSWALK> 
inline CROSSWALK& GenericCrossRoads<CROSSWALK>::right()  {
	return rightcrosswalk;
}

template<class CROSSWALK> 
inline bool GenericCrossRoads<CROSSWALK>::construct(Controller* controller){
	bool retvalue=Model::construct(controller);
	if (retvalue) {
		// --Setting of the ports

		// --Main traffic lights
		left().main().port(O1,O2,O3);
		right().main().port(O4,O5,O6);
		bottom().main().port(O9,O10,O11);
		top().main().port(O12,O13,O14);

		// --Pedestrian lights
		left().right().port(O17,O18);
		right().right().port(O19,O20);
		bottom().right().port(O21,O22);
		top().right().port(O23,O24);
		
		left().left().port(O25,O26);
		right().left().port(O27,O28);
		bottom().left().port(O29,O30);
		top().left().port(O31,O32);

		// --Other lights
		camera().port(O15);
		lighting().port(O16);

		// --Setting the buttons and the contact sensors
		left().right().button().port(I1);	
		left().left().button().port(I2);
		right().right().button().port(I3);
		right().left().button().port(I4);
		bottom().right().button().port(I5);
		bottom().left().button().port(I6);
		top().right().button().port(I7);
		top().left().button().port(I8);

		left().stop().port(I9);
		right().stop().port(I10);
		bottom().stop().port(I11);
		top().stop().port(I12);

		contactRight().port(I13);
		contactLeft().port(I14);
		
		lightness().port(A1);

		// --Set the controller
		left().construct(controller);
		right().construct(controller);
		top().construct(controller);
		bottom().construct(controller);
		contactRight().Controlled::connect(controller);
		contactLeft().Controlled::connect(controller);
		lightness().Controlled::connect(controller);
		camera().Controlled::connect(controller);
		lighting().Controlled::connect(controller);

	}
	return retvalue;
}


template<class CROSSWALK> 
inline CROSSWALK& GenericCrossRoads<CROSSWALK>::left(){
	return leftcrosswalk;
}

template<class CROSSWALK> 
inline CROSSWALK& GenericCrossRoads<CROSSWALK>::bottom(){
	return bottomcrosswalk;
}

template<class CROSSWALK> 
inline CROSSWALK& GenericCrossRoads<CROSSWALK>::top(){
	return topcrosswalk;
}

template<class CROSSWALK> 
inline Lamp& GenericCrossRoads<CROSSWALK>::camera(){
	return speedcamera;
}

template<class CROSSWALK> 
inline Lamp& GenericCrossRoads<CROSSWALK>::lighting(){
	return roadlighting;
}

template<class CROSSWALK> 
inline Photo& GenericCrossRoads<CROSSWALK>::lightness(){
	return photolight;
}

template<class CROSSWALK> 
inline Contact& GenericCrossRoads<CROSSWALK>::contactLeft(){
	return contactleft;
}

template<class CROSSWALK> 
inline Contact& GenericCrossRoads<CROSSWALK>::contactRight(){
	return contactright;
}

template<class CROSSWALK> 
inline void GenericCrossRoads<CROSSWALK>::test(){
	// --Switch the lights to green.
	right().main().greenOn();
	left().main().greenOn();
	top().main().greenOn();
	bottom().main().greenOn();
	right().left().greenOn();
	right().right().greenOn();
	left().left().greenOn();
	left().right().greenOn();
	top().left().greenOn();
	top().right().greenOn();
	bottom().left().greenOn();
	bottom().right().greenOn();
}

template<class CROSSWALK> 
inline void GenericCrossRoads<CROSSWALK>::test2(){
	// --Switch the lights to red.
	right().main().redOn();
	left().main().redOn();
	top().main().redOn();
	bottom().main().redOn();
	right().left().redOn();
	right().right().redOn();
	left().left().redOn();
	left().right().redOn();
	top().left().redOn();
	top().right().redOn();
	bottom().left().redOn();
	bottom().right().redOn();
}


template<class CROSSWALK> 
inline void GenericCrossRoads<CROSSWALK>::test3(){
	// --Switch the lights to yellow.
	right().main().yellowOn();
	left().main().yellowOn();
	top().main().yellowOn();
	bottom().main().yellowOn();
	lighting().on();
	camera().on();
}


#endif // !defined(EA_31C4818E_B02D_4b90_B7C2_58CB7BEA9BCB__INCLUDED_)
