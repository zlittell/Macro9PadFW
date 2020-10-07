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

extern DeviceInputs InputState;
extern DeviceProfile MacropadProfile;

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

struct KBStateStructure
{
	uint16_t B1 : 1;
	uint16_t B2 : 1;
	uint16_t B3 : 1;
	uint16_t B4 : 1;
	uint16_t B5 : 1;
	uint16_t B6 : 1;
	uint16_t B7 : 1;
	uint16_t B8 : 1;
	uint16_t B9 : 1;
	uint16_t CLEANUP : 1;
	uint16_t : 6;
}__attribute__((packed));

union KBStateTrack
{
	uint16_t STATE;
	struct KBStateStructure BITS;
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
	}

	/*------------- Keyboard -------------*/
	if (tud_hid_n_ready(KeyboardInterface))
	{
		static union KBStateTrack KBState = {0};
		MacroPad_KeyboardReport kbReport = {0};
		
		/*------------- Button1 -------------*/
		if (InputState.Button1)
		{
			kbReport.modifier |= MacropadProfile.Button1.Modifier;
			kbReport.buttons[0] = MacropadProfile.Button1.Button;
			KBState.BITS.B1 = 1;
		}
		else
		{
			if(KBState.BITS.B1)
			{
				KBState.BITS.B1 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button2 -------------*/
		if (InputState.Button2)
		{
			kbReport.modifier |= MacropadProfile.Button2.Modifier;
			kbReport.buttons[1] = MacropadProfile.Button2.Button;
			KBState.BITS.B2 = 1;
		}
		else
		{
			if(KBState.BITS.B2)
			{
				KBState.BITS.B2 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button3 -------------*/
		if (InputState.Button3)
		{
			kbReport.modifier |= MacropadProfile.Button3.Modifier;
			kbReport.buttons[2] = MacropadProfile.Button3.Button;
			KBState.BITS.B3 = 1;
		}
		else
		{
			if(KBState.BITS.B3)
			{
				KBState.BITS.B3 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button4 -------------*/
		if (InputState.Button4)
		{
			kbReport.modifier |= MacropadProfile.Button4.Modifier;
			kbReport.buttons[3] = MacropadProfile.Button4.Button;
			KBState.BITS.B4 = 1;
		}
		else
		{
			if(KBState.BITS.B4)
			{
				KBState.BITS.B4 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button5 -------------*/
		if (InputState.Button5)
		{
			kbReport.modifier |= MacropadProfile.Button5.Modifier;
			kbReport.buttons[4] = MacropadProfile.Button5.Button;
			KBState.BITS.B5 = 1;
		}
		else
		{
			if(KBState.BITS.B5)
			{
				KBState.BITS.B5 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button6 -------------*/
		if (InputState.Button6)
		{
			kbReport.modifier |= MacropadProfile.Button6.Modifier;
			kbReport.buttons[5] = MacropadProfile.Button6.Button;
			KBState.BITS.B6 = 1;
		}
		else
		{
			if(KBState.BITS.B6)
			{
				KBState.BITS.B6 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button7 -------------*/
		if (InputState.Button7)
		{
			kbReport.modifier |= MacropadProfile.Button7.Modifier;
			kbReport.buttons[6] = MacropadProfile.Button7.Button;
			KBState.BITS.B7 = 1;		}
		else
		{
			if(KBState.BITS.B7)
			{
				KBState.BITS.B7 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button8 -------------*/
		if (InputState.Button8)
		{
			kbReport.modifier |= MacropadProfile.Button8.Modifier;
			kbReport.buttons[7] = MacropadProfile.Button8.Button;
			KBState.BITS.B8 = 1;
		}
		else
		{
			if(KBState.BITS.B8)
			{
				KBState.BITS.B8 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		/*------------- Button9 -------------*/
		if (InputState.Button9)
		{
			kbReport.modifier |= MacropadProfile.Button9.Modifier;
			kbReport.buttons[8] = MacropadProfile.Button9.Button;
			KBState.BITS.B9 = 1;
		}
		else
		{
			if(KBState.BITS.B9)
			{
				KBState.BITS.B9 = 0;
				KBState.BITS.CLEANUP = 1;
			}
		}
		
		if (KBState.STATE)
		{
			tud_hid_n_report(KeyboardInterface, 0, &kbReport, sizeof(kbReport));
		}
		
		KBState.BITS.CLEANUP = 0;
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