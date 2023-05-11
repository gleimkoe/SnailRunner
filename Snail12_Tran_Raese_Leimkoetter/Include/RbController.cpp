///////////////////////////////////////////////////////////
//  RbController.cpp
//  Implementation of the Class RbController
//  Created on:      16-Jan-2010 14:48:38
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

// --Direct access of the ftlib (no umfish required!)
#include <windows.h>
#include <assert.h>

#include "rb/FtLibDll.h"            
#include "rb/FtLibDllFunc.h"       

#include "RbController.h"

bool RbController::isLibLoaded=false;

RbController::RbController():rbHandle(NULL),dtype(NO_DEVICE),major_version((unsigned)-1),
minor_version((unsigned)-1),snumber(0),sfirmware(0),smanufacturer(0),slongname(0),sshortname(0) {
	// --Check if lib is loaded.
	if (isLibLoaded==false) {
		DWORD dwStatus = LoadFtLibDll();  
		if (dwStatus==FTLIB_ERR_SUCCESS) {
			// --Init the lib.
			dwStatus = InitFtLib();
			if (dwStatus==FTLIB_ERR_SUCCESS)  {
				// --Init the USB devices.
				dwStatus = InitFtUsbDeviceList();
				if (dwStatus==FTLIB_ERR_SUCCESS) {
					isLibLoaded=true;
				}
			}
		}	
	}
	assert(isLibLoaded);
	// --Version
	DWORD dwStatus = GetLibVersion();    
	major_version=(dwStatus&0x0000ff00)>>8;
	minor_version=dwStatus&0x000000ff;
	// --USB-Devices
	usb_devices=GetNumFtUsbDevice();
}

RbController::~RbController(){

}

bool RbController::open(unsigned char channel){
	assert(isLibLoaded);
	bool retvalue=false;
	// --Just USB supported
	if (channel>=0 && channel<usb_devices)
		if ((rbHandle=GetFtUsbDeviceHandle(channel))!=NULL)
			if (OpenFtUsbDevice(rbHandle)==FTLIB_ERR_SUCCESS) {
				// --Identify the type of the device.
				DWORD dwStatus=GetFtDeviceTyp(rbHandle);
				switch (dwStatus) {
					case FT_AUTO_TYPE                : dtype=CROSSROADS; break;
					case FT_INTELLIGENT_IF           : dtype=INTELLIGENT_IF; break;
					case FT_INTELLIGENT_IF_SLAVE     : dtype=INTELLIGENT_IF_SLAVE; break;
					case FT_ROBO_IF_IIM              : dtype=ROBO_IF_IIM; break;
					case FT_ROBO_IF_USB              : dtype=ROBO_IF_USB; break;
					case FT_ROBO_IF_COM              : dtype=ROBO_IF_COM; break;
					case FT_ROBO_IF_OVER_RF          : dtype=ROBO_IF_OVER_RF; break;
					case FT_ROBO_IO_EXTENSION        : dtype=ROBO_IO_EXTENSION; break;
					case FT_ROBO_LT_CONTROLLER       : dtype=ROBO_LT_CONTROLLER; break;
					case FT_ROBO_RF_DATA_LINK        : dtype=ROBO_RF_DATA_LINK; break;
					case FT_SOUND_AND_LIGHTS         : dtype=SOUND_AND_LIGHTS; break;
					case ROBO_CONNECT_BOX            : dtype=CONNECT_BOX; break;
					case EDU_LINE_MODULE             : dtype=MODULE; break;
					case EDU_LINE_CROSSROADS         : dtype=CROSSROADS; break;
					case EDU_LINE_TRAFFICLIGHTS      : dtype=TRAFFICLIGHTS; break;
					case EDU_LINE_SIGNAL             : dtype=SIGNAL; break;
					default                          : dtype=NO_DEVICE;
				};
				// --Seriennummer
				snumber=GetFtSerialNrStrg(rbHandle);
				smanufacturer=GetFtManufacturerStrg(rbHandle);
				sfirmware=GetFtFirmwareStrg(rbHandle);
				slongname=GetFtLongNameStrg(rbHandle);
				sshortname=GetFtShortNameStrg(rbHandle);
				// --Start the Transferarea
				if (StartFtTransferArea(rbHandle,NULL)==FTLIB_ERR_SUCCESS)
					retvalue=true;
			}
	return retvalue;
}


bool RbController::close(){
	return false;
}


bool RbController::input(InputPort iPort, int& nValue, UniversalInput iType){
	bool retvalue=false;
	// --Check if Controller is already initialized.
	if (rbHandle!=NULL) {
		retvalue=true;
		// --Select function based on input type.
		switch (iType) {
			case ANALOG_5KOMEGA : 
				/* nValue=rbGetAnalog(rbHandle,iPort+1); */
				break;
			case DIGITAL_5KOMEGA : {
				unsigned char digital=0;
				FT_TRANSFER_AREA *pTA= GetFtTransferAreaAddress(rbHandle);
				switch (iPort/8) {
					case 0 : digital=pTA->E_Main; break;
					case 1 : digital=pTA->E_Sub1; break;
					case 2 : digital=pTA->E_Sub2; break;
					case 3 : digital=pTA->E_Sub3; break;
				}
				/* --Get the value. */
				nValue=( digital&(1<<(iPort%8)) ? 1 : 0);
				}; 
				break;
			case ANALOG_10KV : {
				FT_TRANSFER_AREA *pTA= GetFtTransferAreaAddress(rbHandle);
				switch (iPort) {
					case A1: nValue=pTA->A1; break;
					case A2: nValue=pTA->A2; break;
					case A3: nValue=pTA->AX; break;
					case A4: nValue=pTA->AY; break;
					default: nValue=-1;
				}
				}; break;
			case DIGITAL_10KV : 
				retvalue=false;	// --zzt. nicht unterstützt
				break;
			case DISTANCE : 
			/*	nValue=rbGetDistanceValue(rbHandle,iPort); */ retvalue=false;
				break;
			case COUNTER :
				/* nValue=rbGetCounter(rbHandle,iPort+1); */
				break;
			default :
				retvalue=false;
		};
	}
	return retvalue;
}


bool RbController::output(MotorPort /*oport*/, Direction /*dir*/, int /*speed*/){
	bool retvalue=false;
#if 0
	// --Check if Controller is initialized.
	if (isInitialized()) {
		int x=7;
		retvalue=(rbSetMotorEx(rbHandle,oport,dir==LEFT ? 1 : 2,x) == rbFehler ? false : true);
	}
#endif
	return retvalue;
}


bool RbController::output(OutputPort oport, int value){
	assert(isLibLoaded);
	bool retvalue=false;
	if (rbHandle!=NULL) {
		FT_TRANSFER_AREA *pTA= GetFtTransferAreaAddress(rbHandle);
		unsigned char* pwm=0;			
		unsigned char* polarity=0;
		/* --Assign pointer to the master/sub module. */
		switch (oport/8) {
			case 0 : pwm=pTA->MPWM_Main; polarity=&(pTA->M_Main); break;
			case 1 : pwm=pTA->MPWM_Sub1; polarity=&(pTA->M_Sub1); break;
			case 2 : pwm=pTA->MPWM_Sub2; polarity=&(pTA->M_Sub2); break;
			case 3 : pwm=pTA->MPWM_Sub3; polarity=&(pTA->M_Sub3); break;
		};

		/* --Enable or disable? */
		if (value==0) {
			/* --Disable the port. */
			*polarity&=~(1<<(oport%8));
		}
		// --Enable 
		else {
			/* --Enable the port. */
			*polarity|=(1<<(oport%8));
			/* --Set the value. */
			pwm[oport%8]=unsigned char(value-1);
		}
		retvalue=true;
	}
		
	return retvalue;
}


bool RbController::clear(InputPort /*cPort*/){
	bool retvalue=false;
	// --Check if controller is initialized.
#if 0
	if (isInitialized())
		retvalue=(rbSetCounter(rbHandle,cPort,0) == rbFehler ? false : true);
#endif
	return retvalue;
}

bool RbController::configure(InputPort /*iPort*/, UniversalInput /*iType*/, std::string /*info*/) {
	return true;
}

bool RbController::configure(MotorPort) {
	return true;
}

bool RbController::configure(OutputPort) {
	return true;
}