/**
* @file main.c
* @brief Main application code.
* @author Zack Littell
* @company Mechanical Squid Factory
* @project Macro9Pad
*/
#include "sam.h"
#include "main.h"
#include "sys_samd11.h"
#include "tusb.h"
#include "USBCallbacks.h"
#include "MSF_I2C/MSF_I2C.h"
#include "MSF_RGB/MSF_RGB.h"
#include "macropad.h"

/**
	@brief Application Main
	@details Main function for application.
	@returns int from main
*/
int main(void)
{
	/* Initialize the SAM system */
	SystemInit();

	// Initialize Device
	configureClocks();
	init_IO();
	init_TC2();
	init_i2c();
	init_Memory();
	
	// System Startup
	LED_init(LEDADDR);
	LoadProfile();
	enable_interrupts();
	tusb_init();

	// Application Loop
	while (1)
	{
		tud_task();
		hid_task();
	}
}
