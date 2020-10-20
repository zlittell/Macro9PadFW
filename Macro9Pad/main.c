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
	
	//Configure NVM
	NVMCTRL->CTRLB.reg = (NVMCTRL_CTRLB_CACHEDIS | NVMCTRL_CTRLB_MANW | NVMCTRL_CTRLB_RWS(0));
	
	//debugEnableClockOutputs();

	
	configureClocks();
	init_IO();
	init_TC2();
	init_i2c();

	enable_interrupts();
	
	LED_init();
	LoadProfile();
	SaveProfile();
	
	tusb_init();

	/* Replace with your application code */
	while (1)
	{
		__NOP();
		tud_task();
		hid_task();
	}
}
