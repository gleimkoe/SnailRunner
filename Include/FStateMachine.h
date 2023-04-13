#ifndef _FSTATEMACHINE_H
#define _FSTATEMACHINE_H

#include <fstream>
#include <iomanip>

/*! Diese Template-Klasse definiert die Basisklasse für Zustandsmaschinen. Hierbei
 *  werden ein paar Grundfunktionen bereitgestellt, die das Arbeiten und die Fehlersuche
 *  mit/in Zustandsmaschinen vereinfachen. Die Basisklasse erwartet einen Template-Parameter,
 *  dieser wird in der Programmierung als 'Trait' bezeichnet, der die nachfolgenden Anforderungen
 *  erfüllen muss:
 *
 *  struct XYZ_Trait {
 *
 *	enum State { INITIAL, FINAL, FAILURE, ZUSTAND_1, ZUSTAND_2, ... , ZUSTAND_N, NSTATE };
 *	enum Event { EREIGNIS_1, EREIGNIS_2, ...., EREIGNIS_M, NEVENT };
 *	
 *	typedef ABC Model;
 *
 *	static const char* const Filename;
 *
 *	static const char* const StateDescr[NSTATE];
 *	static const char* const EventDescr[NEVENT];
 * };
 *
 * XYZ ist durch einen geeigneten Namen zu ersetzen. Jede Zustandsmaschine muss mindestens die
 * Zustände INITIAL, FINAL und FAILURE defineren. Hinzukommen eine beliebige Anzahl weiterer Zustände.
 * Der letzte Eintrag in der Aufzählung muss NSTATE sein.
 *
 * ABC ist durch den Typen des Fischertechnik-Modells zu ersetzen und muss eine abgeleitete Klasse
 * von ReactiveModel sein. Die statischen Attribute Filename, StateDescr und EventDescr enthalten den
 * Namen der Datei sowie die Namen der Zustände und Ereignisse als Zeichenkette. Diese werden für
 * Debugzwecke verwendet.
 */

template<typename T> class FishermanStateMachine {
public:
	
	/*! Typen-Kurzschreibeweise für Zustände. */
	typedef typename T::State State; 
	/*! Typen-Kurzschreibweise für Ereignisse. */
	typedef	typename T::Event Event; 
	/*! Typen-Kurzschreibweise das Fischertechnik-Modell. */
	typedef typename T::Model Model; 

	/*! Konstruktur der Basisklasse.
	 * \param m gibt das Fischertechnik-Modell an.
	 * \param debug gibt an, ob die Zustandsmaschine Debug-Informationen in eine Datei schreiben soll. */
	FishermanStateMachine(Model* m =0, bool debug =true);

	/*! Desktruktor. */
	virtual ~FishermanStateMachine();

	/*! Diese Methode liefert den aktuellen Zustand der Zustandsmaschine zurück. */
	State state() const { return current; }

	/*! Diese Methode realisiert einen Zustandswechsel und muss in abgeleiteten Klassen
	 *  überladen und mit einer korrekten Zustandsübergangsmatrix (z.B. durch switch-case)
	 *  implementiert werden.
	 *  \param ev gibt das Ereignis an, welches einen möglichen Zustandswechsel auslöst. */
	virtual void transition(Event ev);

	/*! Diese Methode sollte aufgerufen werden, wenn die Zustandsmaschine den ersten Zustandswechsel
	 *  vom initialen Ausgangszustand durchführen soll. Diese Methode muss in abgeleiteten Klassen
	 *  überladen werden. Üblicherweise sollte in dieser Methode eine onEnteringXYZ()-Methode der
	 *  konkreten Zustandsmaschine gewechselt werden. */
	virtual void onLeavingInitial();

	/*! Diese Methode sollte aufgerufen werden, wenn die Zustandsmaschine vollständig durchlaufen 
	 *  ist. In einer abgeleiteten Klasse können beispielsweise Variablen zurückgesetzt werden oder
	 *  ein Text auf der Konsole ausgegeben werden. */
	virtual void onEnteringFinal();

	/*! Diese Methode sollte aufgerufen werden, wenn die Zustandsmaschine in einen Fehlerzustand 
	 *  wechselt. In abgeleiteten Klassen kann diese überladen werden, um eine spezielle Fehlerfunktion
	 *  auszuführen. In der Implementierung der Basisklasse wird die failure()-Methode des Fischertechnik-
	 *  Modells aufgerufen. */
	virtual void onEnteringFailure();

	/*! Analog zur onEnteringFailure()-Methode sollte diese Methode aufgerufen werden, wenn der Fehlerzustand
	 *  wieder verlassen wird. In der Implementierung der Basisklasse wird die recover()-Methode des
	 *  Fischertechnik-Modells aufgerufen. */
	virtual void onLeavingFailure();

	/*! Hiermit wird das Logging der Zustandswechsel eingeschaltet (siehe auch Konstruktor). */
	void enableDebug();

	/*! Hiermit wird das Logging der Zustandswechsel ausgeschaltet (siehe auch enableDebug()). */
	void disableDebug();

protected:

	/*! Die Methode setzt den aktuellen Zustand auf einen neuen Wert. Ist das Logging aktiviert,
	 *  dann wird der Zustandwechsel in eine Datei geschrieben.
	 *  \param nstate gibt den neuen Zustand an. */
	void state(State nstate);

	/*! Die Methode schreibt einen Zustand in die Logging-Datei (für Debugzwecke). 
	 *  \param state gibt des Zustand an. */
	void print(State state);

	/*! Die Methode schreibt ein Ereignis in die Logging-Datei (für Debugzwecke). 
	 *  \param ev gibt das Ereignis an. */
	void print(Event ev);

	/*! Die Methode schreibt eine Zeichenkette in die Logging-Datei (für Debugzwecke). 
	 *  \param message gibt die Zeichenkette an. */
	void print(const char* message);		

	Model* model; /*! <Zeiger auf das Fischertechnik-Modell, welches durch die Zustandsmachine gesteuert wird. */
	
	/* --! Avoid assigment. */
	FishermanStateMachine<T>& operator=(const FishermanStateMachine<T>&) { return *this; }
	
private:

	State current;
	bool isDebug;
	std::ofstream* out;

};

/* --Template-Implementierung. */
template<typename T> FishermanStateMachine<T>::FishermanStateMachine(Model* m, bool debug):
	model(m),current(State::INITIAL),isDebug(debug),out(0) {}

template<typename T> FishermanStateMachine<T>::~FishermanStateMachine() {
	if (out) {
		out->close();
		delete out;
	}
}

template<typename T> void FishermanStateMachine<T>::transition(Event ev) {
	print(ev);
	onEnteringFailure();
}

template<typename T> void FishermanStateMachine<T>::onLeavingInitial() {
	current=State::INITIAL;
	if (out) {
		out->close();
		delete out;
	}
	/* --Öffne Datei für Debug-Zwecke. */
	out=new ofstream(T::Filename);
	/* --Initialen Zustand rausschreiben. */
	print(current);
}

template<typename T> void FishermanStateMachine<T>::onEnteringFinal() {
	state(State::FINAL);
}

template<typename T> void FishermanStateMachine<T>::onEnteringFailure() {
	state(State::FAILURE);
	/* --Modell in den failure-Modus versetzen. */
	model->failure();
}

template<typename T> void FishermanStateMachine<T>::onLeavingFailure() {
	/* --Modell aus dem failure-Modus zurücksetzen. */
	model->recover();
}

template<typename T> void FishermanStateMachine<T>::enableDebug() {
	isDebug=true;
}

template<typename T> void FishermanStateMachine<T>::disableDebug() {
	isDebug=false;
}

template<typename T> void FishermanStateMachine<T>::state(State nstate) {
	current=nstate;
	print(current);
}

template<typename T> void FishermanStateMachine<T>::print(State state) {
	if (isDebug && out) 
		*out << std::setw(10) << model->current() << " NEW STATE: "
		     << std::setw(2) << (int)state << '|' << std::setw(20) << T::StateDescr[state]
	         << std::endl;
}


template<typename T> void FishermanStateMachine<T>::print(Event ev) {
	if (isDebug && out) 
		*out << std::setw(10) << model->current() << " NEW EVENT: "
		     << std::setw(2) << (int)ev << '|' << std::setw(20) << T::EventDescr[ev]
	         << std::endl;
}

template<typename T> void FishermanStateMachine<T>::print(const char* message) {
	if (isDebug && out) 
		*out << std::setw(10) << model->current() << " MESSAGE  : " << message
	         << std::endl;
}


#endif
