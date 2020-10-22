/**
* @file USBCallbacks.c
* @brief Code for application specific USB functionality
* @author Zack Littell
* @company Mechanical Squid Factory
* @project Macro9Pad
*/

#include <tusb.h>
#include "macropad.h"

static bool remote_wakeup_supported = false;

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

/**
	@brief Mount Callback
	@details Callback invoked when device is "mounted"
*/
void tud_mount_cb(void)
{
}

/**
	@brief Unmount Callback
	@details Callback invoked when device is "unmounted"
*/
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
/**
	@brief Suspend Callback
	@details Callback invoked when device is suspended, device must draw an average of current
		less than 2.5mA from bus within 7ms.
	@param[in] remote_wakeup_en indicates whether host allows us to perform remote wakeup
*/
void tud_suspend_cb(bool remote_wakeup_en)
{
	remote_wakeup_supported = remote_wakeup_en;
	SleepDevice();
}

/**
	@brief Resume Callback
	@details Callback invoked when device is resumed
*/
void tud_resume_cb(void)
{
	WakeDevice();
}

/**
	@brief Wake USB
	@details Checks if wake is supported and if device is suspended
*/
void WakeUSB(void)
{
	// Remote wakeup
	if ( tud_suspended() && remote_wakeup_supported)
	{
		// Wake up host if we are in suspend mode
		// and REMOTE_WAKEUP feature is enabled by host
		tud_remote_wakeup();
	}
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
enum
{
	ConfigInterface = 0,	///< This is the HID interface for configuration
	KeyboardInterface = 1	///< This is the HID interface for the keyboard
};

/**
	@brief Hid Task
	@details Periodic task for handling HID interface
*/
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
		switch(CommandBufferProcess())
		{
			case (CMD_SendProfile):
			{
				uint8_t sendBuffer[PROFILE_MESSAGE_LENGTH+1];
				CopyProfileToBuffer(sendBuffer);
				tud_hid_n_report(ConfigInterface, 0, sendBuffer, (PROFILE_MESSAGE_LENGTH+1));
			}
		}
	}

	/*------------- Keyboard -------------*/
	if (tud_hid_n_ready(KeyboardInterface))
	{
		MacroPad_KeyboardReport kbReport = {0};
		
		if (ProcessInputs(&kbReport))
		{
			WakeUSB();
			tud_hid_n_report(KeyboardInterface, 0, &kbReport, sizeof(kbReport));
		}
	}
}

/**
	@brief Get Report Callback
	@details Received GET_REPORT control request from host, application must fill buffer report's content and return its length.
	@note Return zero will cause the stack to STALL request.
	@param[in] itf Interface on which the control request was received
	@param[in] report_id ReportID of the control request
	@param[in] report_type type of report
	@param[out] buffer USB data buffer
	@param[in] reqlen Length of request
	@returns Returns length of data filled in buffer
*/
uint16_t tud_hid_n_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
	// TODO not Implemented
	(void) itf;
	(void) report_id;
	(void) report_type;
	(void) buffer;
	(void) reqlen;

	return 0;
}

/**
	@brief Set Report Callback
	@details Invoked when SET_REPORT control request is received or 
		data is received on OUT endpoint.
	@param[in] itf Interface on which the request was received
	@param[in] report_id ReportID of the request
	@param[in] report_type type of report
	@param[in] buffer USB data buffer
	@param[in] bufsize Size of USB data buffer
*/
void tud_hid_n_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
	(void) report_id;
	(void) report_type;
	
	if(itf == ConfigInterface)
	{
		if (bufsize)
		{
			CommandParse(buffer, bufsize);
		}
	}
	else if(itf == KeyboardInterface)
	{
		// CAPLOCK, NUMLOCK etc...
	}
}