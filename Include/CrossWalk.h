///////////////////////////////////////////////////////////
//  CrossWalk.h
//  Implementation of the Class CrossWalk
//  Created on:      21-Mai-2010 11:53:23
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_09611158_F115_4625_A99B_BC864A6311D6__INCLUDED_)
#define EA_09611158_F115_4625_A99B_BC864A6311D6__INCLUDED_

#include "TrafficLight.h"
#include "PedestrianLight.h"
#include "Contact.h"
#include "Model.h"

/*! Die Template-Klasse GenericCrossWalk beschreibt einen Fußgängerüberweg in
 *  dem CrossRoads-Modell. Ein Fußgängerüberweg besteht aus einer Straßenampel
 *  und 2 Fußgängerampeln, die links und rechts der Straße angeordnet sind.
 *  Zusätzlich befindet sich vor der Haltelinie ein Kontaktsensor.
 *  Hinweis: Für die Verwendung einer konkreten Klasse ist die Klasse CrossWalk
 *  definiert, die die Klassen TrafficLight und PedestrianLight verwendet.
 */

template<class TRAFFIC, class PEDESTRIAN> class GenericCrossWalk : public Model
{

public:

	/*! Der Standardkonstruktor. */
	GenericCrossWalk();
	/*! Der Destruktor. */
	virtual ~GenericCrossWalk();

	/*! Mit dieser Methode wird die Verbindung zwischen dem Fußgängerüberweg und dem 
	 * Controller hergestellt. */
	virtual bool construct(Controller* controller);
	
	/*! Diese Methode liefert die Straßenampel zurück. */
	TRAFFIC& main();
	/*! Diese Methode liefert die linke (in Fahrtrichtung) Fußgängerampel zurück. */
	PEDESTRIAN& left();
	/*! Diese Methode liefert die rechte (in Fahrtrichtung) Fußgängerampel zurück. */
	PEDESTRIAN& right();
	/*! Diese Methode liefert den Kontaktsensor vor der Haltelinie zurück. */
	Contact& stop();
	
private:
	TRAFFIC traffic;
	PEDESTRIAN pedestrianright;
	PEDESTRIAN pedestrianleft;
	Contact stopcontact;

};

/*! Diese Klasse definiert einen Fußgängerüberweg mit den beiden konkreten
 *  Implementierungen für die Straßenampel und den beiden Fußgängerüberwegen.
 */
typedef GenericCrossWalk<TrafficLight,PedestrianLight> CrossWalk;


template<class TRAFFIC,class PEDESTRIAN> 
inline GenericCrossWalk<TRAFFIC,PEDESTRIAN>::GenericCrossWalk(){}

template<class TRAFFIC,class PEDESTRIAN> 
inline GenericCrossWalk<TRAFFIC,PEDESTRIAN>::~GenericCrossWalk(){}

template<class TRAFFIC,class PEDESTRIAN> 
inline TRAFFIC& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::main(){
	return traffic;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline PEDESTRIAN& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::left(){
	return pedestrianleft;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline PEDESTRIAN& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::right(){
	return pedestrianright;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline Contact& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::stop(){
	return stopcontact;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline bool GenericCrossWalk<TRAFFIC,PEDESTRIAN>::construct(Controller* controller){
	bool retvalue=Model::construct(controller);
	if (retvalue) {
		main().construct(controller);
		left().construct(controller);
		right().construct(controller);
		stop().Controlled::connect(controller);
	}
	return retvalue;
}

#endif // !defined(EA_09611158_F115_4625_A99B_BC864A6311D6__INCLUDED_)
