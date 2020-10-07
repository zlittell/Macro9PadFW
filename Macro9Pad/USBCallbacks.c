/*
 * USBCallbacks.c
 *
 * Created: 9/15/2020 4:13:15 PM
 *  Author: zlitt
 */ 

#include <sam.h>
#include <tusb.h>
#include "SystemStructures.h"
#include "USBCallbacks.h"
#include "macropad.h"

void USB_Handler(void)
{
	tud_int_handler(0);
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
	(void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
enum
{
	ConfigInterface = 0,
	KeyboardInterface = 1
};

void hid_task(void)
{
	/*
	uint8_t wakeup = 0;
	// Remote wakeup
	if ( tud_suspended() && wakeup)
	{
		// Wake up host if we are in suspend mode
		// and REMOTE_WAKEUP feature is enabled by host
		tud_remote_wakeup();
	}
	*/

	/*------------- Config Interface -------------*/
	if (tud_hid_n_ready(ConfigInterface))
	{
		//Process CMDs
	}

	/*------------- Keyboard -------------*/
	if (tud_hid_n_ready(KeyboardInterface))
	{
		MacroPad_KeyboardReport kbReport = {0};
		
		if (ProcessInputs(&kbReport))
		{
			tud_hid_n_report(KeyboardInterface, 0, &kbReport, sizeof(kbReport));
		}
	}
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
	// TODO not Implemented
	(void) report_id;
	(void) report_type;
	(void) buffer;
	(void) reqlen;

	return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
	// TODO set LED based on CAPLOCK, NUMLOCK etc...
	(void) report_id;
	(void) report_type;
	(void) buffer;
	(void) bufsize;
}