/*
* Macro9Pad.c
*
* Created: 9/15/2020 1:43:22 PM
* Author : zlitt
*/
#include "sam.h"
#include "main.h"
#include "system_configuration.h"
#include "tusb.h"
#include "USBCallbacks.h"
#include "debug.h"
#include "interrupt_handlers.h"
#include "SystemStructures.h"
#include "MSF_I2C.h"
#include "NXP_PCA9632.h"
#include "macropad.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	
	//debugEnableClockOutputs();
	fillTestProfile();
	
	configureClocks();
	init_IO();
	init_TC2();
	init_i2c();

	enable_interrupts();
	
	tusb_init();
	
	LED_init();
	LED_updateRed(0xFF);
	LED_updateBrightness(0x10);

	/* Replace with your application code */
	while (1)
	{
		__NOP();
		tud_task();
		hid_task();
	}
}
