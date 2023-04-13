#include "SnailRunner.h"

/*! Anschlüsse des Roboters an den TX Controller.
 *  Eingänge:
 *  I1 : Farbsensor hinten
 *  I4 : Farbsensor unten
 *  I6 : Ultraschallsensor
 *	I7 : Kontaktsensor
 *  I8 : Akkuspannung
 *  C1 : Impulszähler rechter Motor
 *  C2 : Impulszähler linker Motor
 *
 *  Ausgänge:
 *  M1 : Rechter Motor
 *  M2 : Linker Motor
 *  O5 : Rechte Lampe
 *  O6 : Linke Lampe
 *  O7 : Front Lampen
 *  O8 : Controller Lampe
 */


#define	INPUT_COLOUR_BACK				I1
#define INPUT_COLOUR_DOWN				I4
#define	INPUT_ULTRASONIC				I6
#define INPUT_PUSH_BUTTON				I7
#define INPUT_ACCU_LEVEL				I8

#define COUNTER_RIGHT_MOTOR				C1
#define COUNTER_LEFT_MOTOR				C2

#define OUTPUT_RIGHT_MOTOR				M1
#define OUTPUT_LEFT_MOTOR				M2
#define OUTPUT_LAMP_RIGHT				O5
#define OUTPUT_LAMP_LEFT				O6
#define OUTPUT_LAMPS_FRONT				O7
#define OUTPUT_LAMP_CONTROLLER			O8

 /* --Diese Informationen sind für die Log-Datei zur Information. */
#define INFO_MOTOR_LEFT		"MOTOR COUNTER|LEFT"
#define INFO_MOTOR_RIGHT	"MOTOR COUNTER|RIGHT"
#define INFO_COLOUR_BACK	"COLOUR|BACK"
#define INFO_COLOUR_DOWN	"COLOUR|FRONT, DOWN"
#define INFO_DISTANCE		"DISTANCE|AHEAD"
#define INFO_ACCU_LEVEL		"ANALOG|POWER SUPPLY, ACCU LEVEL"
#define INFO_BUTTON			"BUTTON|PUSH BUTTON, RIGHT"

SnailRunner::SnailRunner() : leftmotor(INFO_MOTOR_LEFT), rightmotor(INFO_MOTOR_RIGHT), speed(300),
colourSensorback(INFO_COLOUR_BACK),
colourSensordown(INFO_COLOUR_DOWN),
distance(INFO_DISTANCE),
accuLevel(ANALOG_10KV, I1, INFO_ACCU_LEVEL),
pushButton(INFO_BUTTON),
ex_state(0), ob_state(0), se_state(0), fw_state(0), st_state(0), mission(EXPLORE_MISSION) {}


SnailRunner::~SnailRunner(void) {
	delete ob_state;
	delete ex_state;
	delete se_state;
	delete fw_state;
	delete st_state;
}

bool SnailRunner::construct(TxControllerSupervision* controller) {
	bool retvalue = Model::construct(controller);

	/* --Sensoren konfigurieren. */
	if (retvalue == true && distance.connect(INPUT_ULTRASONIC, controller) == false)
		retvalue = false;


	if (retvalue == true && colourSensorback.connect(INPUT_COLOUR_BACK, controller) == false)
		retvalue = false;

	if (retvalue == true && colourSensordown.connect(INPUT_COLOUR_DOWN, controller) == false)
		retvalue = false;

	if (retvalue == true && accuLevel.connect(INPUT_ACCU_LEVEL, controller) == false)
		retvalue = false;


	if (retvalue == true && pushButton.connect(INPUT_PUSH_BUTTON, controller) == false)
		retvalue = false;

	/* --Aktoren konfigurieren. */
	/* --Motoren. */
	if (retvalue == true &&
		(leftmotor.connect(OUTPUT_LEFT_MOTOR, COUNTER_LEFT_MOTOR, controller) == false ||
			rightmotor.connect(OUTPUT_RIGHT_MOTOR, COUNTER_RIGHT_MOTOR, controller) == false))
		retvalue = false;
	else
		/* --Synchronisation der Motoren. */
		leftmotor.synchronize(&rightmotor);

	/* --Lampen. */
	if (retvalue == true &&
		(lamp_control.connect(OUTPUT_LAMP_CONTROLLER, controller) == false ||
			lamp_front.connect(OUTPUT_LAMPS_FRONT, controller) == false ||
			lamp_left.connect(OUTPUT_LAMP_LEFT, controller) == false ||
			lamp_right.connect(OUTPUT_LAMP_RIGHT, controller) == false))
		retvalue = false;

	/* --Zustandsmaschine erzeugen. */
	ex_state = new ExploreStateMachine(this);
	ob_state = new ObstacleStateMachine(this);
	fw_state = new ForwardStateMachine(this);
	se_state = new SearchStateMachine(this);
	st_state = new StartStateMachine(this);

	/* --Interne Attribute setzen. */
	speed = 250;
	last_colour_down = colourdown().value();
	last_dis = ahead().value();

	return retvalue;
}

bool SnailRunner::forward(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, LEFT, LEFT);
}

bool SnailRunner::backward(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, RIGHT, RIGHT);
}

bool SnailRunner::turnleft(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, LEFT, RIGHT);
}

bool SnailRunner::turnright(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, RIGHT, LEFT);
}

bool SnailRunner::stop() {
	return leftmotor.turnOff();
}
void SnailRunner::turn(double grad) {
	double val = 0;
	RunUnit unit = IMPULSE;
	if (grad > 0)
	{
		val = grad * grad_in_impulse * uebersetzungsverhaeltnis_getriebe * radUmdrehungproGrad;
		leftmotor.turnOn(val, unit, speed, RIGHT, LEFT); // wenn Drehwinkel >0 dann dreht rechts
	}
	else if (grad < 0)
	{
		double grad_neu = abs(grad);
		val = grad_neu * grad_in_impulse * uebersetzungsverhaeltnis_getriebe * radUmdrehungproGrad;
		leftmotor.turnOn(val, unit, speed, LEFT, RIGHT); // wenn Drehwinkel <0 dann dreht links
		cout << grad_neu;
	}
	else
	{
		leftmotor.turnOff(); // wenn Drehwinkel gleich Null dann stoppen
	}
}
/*! Diese Methode wird aufgerufen, wenn ein oder mehrere Motoren
 *  am Roboter gestoppt sind. Dieses kann passieren, wenn (i) der
 *  vorgegebene Wert vomn Drehimpulsen erreicht ist, oder (ii) der
 *  Motor explizit gestoppt wurde.
 *
 *  Die Methode überprüft, ob Motor 1 und/oder Motor2 angehalten hat
 *  und erzeugt dass Ereignis "IS_STOPPED", welches dann in der
 *  Zustandsmaschine zu einer Transition (Zustandswechsel) führt.
 *  Das Verhalten des Roboters ist in den Dateien Mission.{cpp|h}
 *  beschrieben. */

void SnailRunner::onMotorStopped(Bitfield bfield) {
	// --Überprüfe, ob die Motoren gestoppt sind.
	if (bfield&((1 << OUTPUT_LEFT_MOTOR) | (1 << OUTPUT_RIGHT_MOTOR)))
		// --Überprüfe welche Mission.
		if (mission == OBSTACLE_MISSION)
			ob_state->handle(ObstacleStateMachine::Event::IS_STOPPED);
		else if (mission == EXPLORE_MISSION)
			ex_state->handle(ExploreStateMachine::Event::IS_STOPPED);
		else if (mission == SEARCH_MISSION)
			se_state->handle(SearchStateMachine::Event::IS_STOPPED);
		else if (mission == FORWARD_MISSION)
			fw_state->handle(ForwardStateMachine::Event::IS_STOPPED);
		else if (mission == START_MISSION && SnailRunner::vergleich(SnailRunner::SollEcken) == false)
			st_state->handle(StartStateMachine::Event::IS_STOPPED);

}

/*! Diese Methode überprüft bestimmte Sensoreingänge. Wenn Schwellenwerte
 *  überschritten werden, dann werden Ereignisse erzeugt und diese
 *  einer Zustandsmaschine übergeben. In der Zustandsmaschine wird dann
 *  eine Transition durchgeführt (in Abhängigkeit der Mission).
 *
 *  Mission 1 (Spurverfolgung)
 *  Wenn der Farbwert (THRESHOLD_COLOR) unterschritten wird, dann wird das
 *  Ereignis OFF_TRAIL ausgelöst. Liegt der aktuelle Farbwert wieder über
 *  dem Schwellenwert, dann wird das Ereignis ON_TRAIL ausgelöst.
 *
 *  Mission 2 (Braunsche Bewegung)
 *  Wenn der Distanzwert (THRESHOLD_DISTANCE_LOW) unterschritten wird, dann
 *  wird das Ereignis WALL_AHEAD ausgelöst. Liegt der aktuelle Distanzwert
 *  wieder oberhalb von THRESHOLD_DISTANCE_HIGH, dann wird das Ereignis
 *  CLEAR_VIEW ausgelöst.
 *
 *  Das eigentliche Verhalten des Roboters ist in den Zustandsmaschinen in
 *  den Dateien Mission.{cpp|h} beschrieben.
 */

void SnailRunner::onInputChanged(Bitfield bfield) {
	const int THRESHOLD_COLOR = 1400;
	const int THRESHOLD_DISTANCE_LOW = 30;
	const int THRESHOLD_DISTANCE_HIGH = 40;
	const int THRESHOLD_DISTANCE = 10;
	const int THRESHOLD_COLOR_GRAU_WEISS = 1200;
	const int THRESHOLD_COLOR_GRAU_SCHWARZ = 2000;

	if (bfield&(1 << INPUT_COLOUR_DOWN)) {
		int col = colourdown().value();
		// --Überprüfe, ob Schwellenwert überschritten.
		if (col > THRESHOLD_COLOR && last_colour_down < THRESHOLD_COLOR) {
			if (mission == EXPLORE_MISSION)
				ex_state->handle(ExploreStateMachine::Event::ON_TRAIL);
			else if (mission == SEARCH_MISSION)
				se_state->handle(SearchStateMachine::Event::ON_TRAIL);
			else if (mission == START_MISSION)
				st_state->handle(StartStateMachine::Event::ON_TRAIL);
		}
		else if (col<THRESHOLD_COLOR && last_colour_down>THRESHOLD_COLOR) {
			if (mission == EXPLORE_MISSION)
				ex_state->handle(ExploreStateMachine::Event::OFF_TRAIL);
			else if (mission == FORWARD_MISSION)
				fw_state->handle(ForwardStateMachine::Event::OFF_TRAIL);
			else if (mission == START_MISSION)
				st_state->handle(StartStateMachine::Event::OFF_TRAIL);
		}
		else if (last_colour_down > THRESHOLD_COLOR_GRAU_SCHWARZ && col > THRESHOLD_COLOR_GRAU_WEISS && col < THRESHOLD_COLOR_GRAU_SCHWARZ) {
			if (mission == START_MISSION)
				st_state->handle(StartStateMachine::Event::ON_GREY);
		}
		last_colour_down = col;
		 
	}

	if (bfield&(1 << INPUT_ULTRASONIC)) {
		if (mission == OBSTACLE_MISSION) {
			// --Überprüfe, ob Schwellwert überschritten.
			int dis = ahead().value();

			if (dis >= THRESHOLD_DISTANCE && last_dis < THRESHOLD_DISTANCE) {
				ob_state->handle(ObstacleStateMachine::Event::CLEAR_VIEW);
			}
			else if (dis < THRESHOLD_DISTANCE && last_dis >= THRESHOLD_DISTANCE) {
				ob_state->handle(ObstacleStateMachine::Event::WALL_AHEAD);

			}

			//evt in die Klammer
			last_dis = dis;
		}
		else if (mission == START_MISSION) {

			int dis = ahead().value();
			cout << "test0" << endl;
			while (st_state->state() == StartStateMachine::State::START)
			{
				dis = ahead().value();
				WaitUntilIsOver(100);
				cout << "test1" << endl;
				if (dis >= THRESHOLD_DISTANCE && last_dis < THRESHOLD_DISTANCE) {
					st_state->handle(StartStateMachine::Event::NOT_WALL_AHEAD);
					cout << "test2" << endl;
				}
				else if (dis < THRESHOLD_DISTANCE && last_dis >= THRESHOLD_DISTANCE) {
					st_state->handle(StartStateMachine::Event::WALL_AHEAD);
					cout << "test3" << endl;
				}

			}
			last_dis = dis;
		}
	}
}


/*! Diese Methode wird zum Beginn des reaktiven Modus des Roboters ausgeführt.
 *  Es wird die Controller-Lampe angeschaltet und dann in den ersten Zustand der
 *  jeweiligen Zustandsmaschine gewechselt. */

void SnailRunner::onStart() {
	lampcontroller().on();
	if (mission == OBSTACLE_MISSION)
		ob_state->start();
	else if (mission == EXPLORE_MISSION)
		ex_state->start();
	else if (mission == FORWARD_MISSION)
		fw_state->start();
	else if (mission == SEARCH_MISSION)
		se_state->start();
	else if (mission == START_MISSION)
		st_state->start();
}

/*! Diese Methode wird beim Beenden des reaktiven Modus des Roboters ausgeführt.
 *  Es wird die blaue Lampe wieder ausgeschaltet. */

void SnailRunner::onStop() {
	lampcontroller().off();
	stop();
}

void SnailRunner::onCounterChanged(Bitfield /*bfield*/) {}

/* -- Vergleich-Funktion -- */
bool SnailRunner::vergleich(int SollEcke) {
	if (SollEcke == st_state->ecke_cnt())
	{
		st_state->handle(StartStateMachine::Event::ECKEN_CNT);
		return true;
	}
	else
	{
		return false;
	}

}