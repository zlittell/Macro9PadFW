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
#include "debug.h"
#include "interrupt_handlers.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	
	//debugEnableClockOutputs();
	
	configureClocks();
	init_IO();
	init_TC2();	//enable this causes chip to constantly reset

	enable_interrupts();
	
	tusb_init();
	
	

	/* Replace with your application code */
	while (1)
	{
		__NOP();
		tud_task();
	}
}
