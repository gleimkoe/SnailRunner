///////////////////////////////////////////////////////////
//  RbController.h
//  Implementation of the Class RbController
//  Created on:      21-Mai-2010 11:53:27
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_30FE3976_8980_4721_AC50_E5904BCD1C6D__INCLUDED_)
#define EA_30FE3976_8980_4721_AC50_E5904BCD1C6D__INCLUDED_

#include "Makros.h"
#include "UniversalInput.h"
#include "InputPort.h"
#include "Direction.h"
#include "MotorPort.h"
#include "OutputPort.h"
#include "Controller.h"

/*! Die RbController-Klasse abstrahiert einen ROBO IF-Controller.
 *
 */

class DLL_DECLSPEC RbController : public Controller
{

public:
	
	/*! Der ROBO IF-Controller kann unterschiedliche Ausprägungen haben,
	 *  die durch den nachfolgenden Aufzählungstyp beschrieben werden. 
	 */
	enum Device { NO_DEVICE, AUTO_TYPE, INTELLIGENT_IF, INTELLIGENT_IF_SLAVE,
		ROBO_IF_IIM, ROBO_IF_USB, ROBO_IF_COM, ROBO_IF_OVER_RF, ROBO_IO_EXTENSION,        
		ROBO_LT_CONTROLLER, ROBO_RF_DATA_LINK, SOUND_AND_LIGHTS, CONNECT_BOX,            
		MODULE, CROSSROADS, TRAFFICLIGHTS, SIGNAL};

	/*! Der Standardkonstruktor. */
	RbController();
	/*! Der Destruktor. */
	virtual ~RbController();

 	virtual bool input(InputPort iPort, int& nValue, UniversalInput iType = ANALOG_5KOMEGA);
	virtual bool output(MotorPort oport, Direction dir, int speed);
	virtual bool output(OutputPort oport, int value);
	virtual bool clear(InputPort cPort);
	virtual bool configure(InputPort iPort, UniversalInput iType = ANALOG_5KOMEGA, std::string info = "unknown");
	virtual bool configure(MotorPort);
	virtual bool configure(OutputPort oport);
	
	/*! Diese Methode öffnet einen ROBO IF-Controller. Es können mehrere
	 *  Geräte am USB gleichzeitig betrieben werden. Die Anzahl der angeschlossenen
	 *  ROBO IF-Controller kann über die usbDevices()-Methode erfragt werden. Wenn
	 *  nur ein Gerät am Rechner angeschlossen ist, dann muss der Parameter 0 sein.
	 * \param channel der Index des USB-Gerätes.
	 */
	bool open(unsigned char channel);

	/*! Diese Methode schliesst ein zuvor geöffneten ROBO IF-Controller. */
	bool close();

	/*! Die Methode liefert die Hauptversion der verwendeten DLL. */
	unsigned int major() const { return major_version; }
	/*! Die Methode liefert die untergeordnete Version der verwendeten DLL. */
	unsigned int minor() const { return minor_version; }
	/*! Die Methode liefert die untergeordnete Version (Klein) der verwendeten DLL. */
	unsigned int sub() const { return 0; }
	/*! Die Methode liefert den Gerätetypen des ROBO IF-Controllers zurück. */
	Device device() const { return dtype; }
	/*! Die Methode liefert die Seriennummer des ROBO IF-Controllers zurück. */
	const char* serialnumber() const { return snumber; }
	/*! Die Methide liefert die Firmware des ROBO IF-Controllers zurück. */
	const char* firmware() const { return sfirmware; }
	/*! Die Methode liefert den Namen des Herstellers des ROBO IF-Controllers zurück. */
	const char* manufacturer() const { return smanufacturer; }
	/*! Die Methode liefert den Namen des Gerätes zurück. */
	const char* name() const { return slongname; }
	/*! Die Methode liefert den Kurznamen des Gerätes zurück. */
	const char* shortname() const { return sshortname; }
	/*! Die Methode liefert die Anzahl der am USB angeschlossenen ROBO IF-Geräte zurück.
	 *  In der open()-Methode können unterschiedliche Geräte geöffnet werden. 
	 */
	unsigned int usbDevices() const { return usb_devices; }

private:
	void* rbHandle;

	// --Attributes of the controller
	Device dtype;
	unsigned int major_version;
	unsigned int minor_version;
	const char* snumber;
	const char* sfirmware;
	const char* smanufacturer;
	const char* slongname;
	const char* sshortname; 
	unsigned int usb_devices;

	// --Lib already loaded
	static bool isLibLoaded;

};
#endif // !defined(EA_30FE3976_8980_4721_AC50_E5904BCD1C6D__INCLUDED_)
