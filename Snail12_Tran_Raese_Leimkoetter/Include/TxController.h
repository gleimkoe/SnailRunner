///////////////////////////////////////////////////////////
//  TxController.h
//  Implementation of the Class TxController
//  Created on:      21-Mai-2010 11:53:30
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_129B6A41_9EF2_40d4_80B8_594355473306__INCLUDED_)
#define EA_129B6A41_9EF2_40d4_80B8_594355473306__INCLUDED_

#include "Makros.h"
#include "UniversalInput.h"
#include "InputPort.h"
#include "Direction.h"
#include "MotorPort.h"
#include "OutputPort.h"
#include "Controller.h"

#include <windows.h>

/*! Die Klasse TxController definiert die Schnittstelle zu dem Fischertechnik TX Controller.
 *  Es werden die pur virtuellen Methoden der Controller-Oberklasse implementiert und einige
 *  weitere zum Abfragen von bestimmten Informationen. Wichtiger Hinweis: Diese Klasse sollte
 *  nicht mehr verwendet werden, sondern die abgeleitete Klasse TxControllerSupervision. */

class DLL_DECLSPEC TxController : public Controller
{

public:
	/*! Der Standardkonstruktor. */
	TxController();
	/*! Der Destruktor. */
	virtual ~TxController();

	/*! Mit dieser Methode wird die Kommunikation zum TX Controller geöffnet. Die meisten
	 *  Methoden dürfen erst dann aufgerufen werden, wenn dieser Befehl erfolgreich ausgeführt
	 *  worden ist. 
	 *  \param com MS Windows COM-Schnittstelle, die mit dem Controller verbunden ist (USB oder Bluetooth).
	 */
	bool open(int com);
	/*! Mit dieser Methode wird die Kommunikation mit dem TX Controller beendet. */
	bool close();
	/*! Die Methode liefert die Hauptversion der verwendeten DLL. */
	unsigned int major() const { return major_version; }
	/*! Die Methode liefert die untergeordnete Version (Mitte) der verwendeten DLL. */
	unsigned int minor() const { return minor_version; }
	/*! Die Methode liefert die untergeordnete Version (Klein) der verwendeten DLL. */
	unsigned int sub() const { return sub_version; }
	/*! Die Methode liefert die Seriennummer des TX-Controllers zurück. */
	const char* serialnumber() const { return snumber; }
	/*! Die Methide liefert die Firmware des TX-Controllers zurück. */
	const char* firmware() const { return sfirmware; }
	/*! Die Methode liefert den Namen des Herstellers des TX-Controllers zurück. */
	const char* manufacturer() const { return smanufacturer; }
	/*! Die Methode liefert den Namen des Gerätes zurück. */
	const char* name() const { return slongname; }
	/*! Die Methode liefert den Kurznamen des Gerätes zurück. */
	const char* shortname() const { return sshortname; }
	/*! Die Methode liefert den Bluetooth-Anschluß zurück. */
	const char* bluetooth() const { return sblue; }
	/*! Die Methode liefert den Namen des Gerätes im Display zurück. */
	const char* display() const { return sdisplay; }
	/*! Die Methode ändert permanent den Namen des Gerätes im Display. Um den
	 *  Namen wieder auf den urspünglichen zurückzusetzen, muss hierzu auf dem
	 *  TX Controller unter "Menu|Settings|Restore defaults" ausgeführt werden. */
	void display(const char* dname);
	/*! Diese Methode gibt einige Informationen zu der Fischertechnik-Software 
	 *  und des verwendeten Controllers in den Ausgabestrom. */
	void banner(ostream& out =cout) const;
	/*! Mit dieser Methode kann eine Nachricht auf dem Display ausgegeben 
	 *  werden. Die Nachricht kann maximal 64 ASCII-Zeichen lang sein. 
	 *  Wird ein leerer String übergeben, dann werden alle Nachrichten
	 *  gelöscht. */
	void message(const char* msg ="");

	HANDLE handle() const;

protected:

	enum { MAX_STRING_LENGTH = 256};

	HANDLE tahandle;

private:
	unsigned int major_version;
	unsigned int minor_version;
	unsigned int sub_version;
	unsigned int usb_devices;
	unsigned int bt_devices;

	char snumber[MAX_STRING_LENGTH];
	char sfirmware[MAX_STRING_LENGTH];
	char smanufacturer[MAX_STRING_LENGTH];
	char slongname[MAX_STRING_LENGTH];
	char sshortname[MAX_STRING_LENGTH];
	char sdisplay[MAX_STRING_LENGTH];
	char sblue[MAX_STRING_LENGTH];

	// --Lib already loaded
	static bool isLibLoaded;

};
#endif // !defined(EA_129B6A41_9EF2_40d4_80B8_594355473306__INCLUDED_)
