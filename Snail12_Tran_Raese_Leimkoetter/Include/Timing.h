#ifndef _TIMING_H
#define _TIMING_H

#include "Makros.h"

/*! Dieser Aufzählungstyp definiert die 8 möglichen Zeitgeber (Timer)
 *  für ein reaktives Fischertechnik-Modell. 
 */

enum TimerIdentifier {
	TIMER1	= 0,
	TIMER2  = 1,
	TIMER3	= 2,
	TIMER4	= 3,
	TIMER5	= 4,
	TIMER6	= 5,
	TIMER7	= 6,
	TIMER8	= 7,
	/* --Anzahl +1. */
	NTIMER
};

/*! Datentyp für Zeitangaben. */

typedef	__int64	TimingInformation;

/*! Vordefinierte Zeitangaben zur besseren Lesbarkeit. */

#define TIMING_NOW			0
#define TIMING_100MS		100
#define TIMING_125MS		125
#define TIMING_250MS		250
#define TIMING_500MS		500
#define TIMING_1S			1000	
#define TIMING_2S			2000	
#define TIMING_5S			5000	
#define TIMING_10S			10000
#define TIMING_1MIN			60000

#define TIMING_SINGLESHOT	0
#define TIMING_FOREVER		-1

/*! Interne Datenstruktur zur Handhabung der Zeitgeber. */

struct DLL_DECLSPEC TimerModel {

	/*! Wahrheitswert, der anggibt, ob der Zeitgeber aktiv ist. */
	bool isActive;

	/*! Periodendauer in [ms] (auch für nicht periodische Zeitgeber). */
	TimingInformation period;

	/*! Auslösezeitpunkt für den nächsten Alarm. */
	TimingInformation alarm;

	/*! Letzter Auslösezeitpunkt oder -1 (TIMING_FOREVER) für periodische Zeitgeber. */
	TimingInformation final;

	/*! Konstruktor mit initialen Werten. */
	TimerModel():isActive(false),period(0),alarm(0),final(TIMING_FOREVER) {}
	
};

#endif
