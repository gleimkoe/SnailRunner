///////////////////////////////////////////////////////////
//  TxController.cpp
//  Implementation of the Class TxController
//  Created on:      16-Jan-2010 14:48:36
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#include "TxController.h"

#include <string.h>

#include <windows.h>
#include <assert.h>

// --12.05.2010: Remove the complete buggy umfish50-stuff (Uelschen)
#include "tx\common.h"
#include "tx\robo_tx_fw.h"
#include "tx\ftmsclib.h"
#include "tx\fterrcode.h"

bool TxController::isLibLoaded=false;

TxController::TxController():tahandle(0),major_version((unsigned)-1),minor_version((unsigned)-1),sub_version((unsigned)-1),
	usb_devices(0),bt_devices(0) {
	// --Check if lib is loaded.
	if (isLibLoaded==false) {
		// --Init the lib.
		DWORD dwStatus=ftxInitLib();
		if (dwStatus==FTLIB_ERR_SUCCESS)  {
			isLibLoaded=true;
		}
	}
	assert(isLibLoaded);
	// --Version
	DWORD dwStatus = ftxGetLibVersion();    
	major_version=(dwStatus&0x00ff0000)>>16;
	minor_version=(dwStatus&0x0000ff00)>>8;
	sub_version=dwStatus&0x000000ff;
	// --USB-Devices
	usb_devices=GetAvailableComPorts(USB_ONLY);
	bt_devices=GetAvailableComPorts(BT_ONLY);
}

TxController::~TxController(){
	ftxCloseLib();
}


#if 0

bool TxController::initialize(){
	bool retvalue=false;
	if (!tahandle) {
		// --Initialize the fischertechnik library
		if (ftxInitLib()==FTLIB_ERR_SUCCESS) {
			retvalue=true;
		}
	}
	return retvalue;
}


bool TxController::finalize(){
	bool retvalue=false;
	// --Close the library
	if (ftxCloseLib()==FTLIB_ERR_SUCCESS)
		retvalue=true;
	return retvalue;
}
#endif

bool TxController::open(int com){
	assert(isLibLoaded);
	bool retvalue=false;
	// --Check if library is already initialized.
	if (!tahandle) {
		DWORD errCode=0;
		// --Open the interface.
		tahandle=ftxOpenComDeviceNr(com,38400,&errCode);
		// --Everything is ok?
		if (tahandle!=NULL && errCode==FTLIB_ERR_SUCCESS) {
			// --Open transfer area.
			errCode=ftxStartTransferArea(tahandle);
			if (errCode==FTLIB_ERR_SUCCESS)
				retvalue=true;
			// --Get the manufacturer string.
			ftxGetManufacturerStrg(tahandle,smanufacturer,MAX_STRING_LENGTH);
			// --Get the short name.
			ftxGetShortNameStrg(tahandle,sshortname,MAX_STRING_LENGTH);
			// --Get the long name.
			ftxGetLongNameStrg(tahandle,slongname,MAX_STRING_LENGTH);
			// --Get the firmware string.
			ftxGetFirmwareStrg(tahandle,sfirmware,MAX_STRING_LENGTH);
			// --Get the serial number.
			ftxGetSerialNrStrg(tahandle,snumber,MAX_STRING_LENGTH);
			// --Get the display name.
			GetRoboTxDevName(tahandle,TA_LOCAL,sdisplay,MAX_STRING_LENGTH);
			// --Get the bluetooth name.
			GetRoboTxBtAddr(tahandle,TA_LOCAL,sblue,MAX_STRING_LENGTH);
		}
	}
	return retvalue;
}


bool TxController::close(){
	bool retvalue=false;	
	// --tahandle ok?
	if (tahandle!=NULL) {
		// --Close the transfer area.
		if (ftxStopTransferArea(tahandle)==FTLIB_ERR_SUCCESS)
			// --Close the device			
			if (ftxCloseDevice(tahandle)==FTLIB_ERR_SUCCESS) {
				tahandle=NULL;			
				retvalue=true;
			}
	}
	return retvalue;	
}

void TxController::display(const char* dname) {
	if (dname) {
		strcpy(sdisplay,dname);
		SetRoboTxDevName(tahandle,TA_LOCAL,sdisplay);
	}
}

void TxController::banner(ostream& out) const {
	Controller::banner(out);
	out << "DISPLAY NAME     : " << display() << endl;
	out << "BLUETOOTH ADDRESS: " << bluetooth() << endl;
}

void TxController::message(const char* msg) {
	SetRoboTxMessage(tahandle,TA_LOCAL,msg);
}

HANDLE TxController::handle() const { return tahandle; }

