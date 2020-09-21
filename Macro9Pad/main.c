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
#include "SystemStructures.h"
#include "MSF_I2C.h"

extern struct DeviceInputs InputState;

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	
	//debugEnableClockOutputs();
	
	configureClocks();
	init_IO();
	init_TC2();	//enable this causes chip to constantly reset
	init_i2c();

	enable_interrupts();
	
	tusb_init();
	
	

	/* Replace with your application code */
	while (1)
	{
		__NOP();
		tud_task();
		
		i2c_start(0x60);
		i2c_write(0x00);
		i2c_write(0x01);
		i2c_write(0x02);
		i2c_write(0x03);
		i2c_write(0x04);
		i2c_write(0x05);
		i2c_write(0x06);
		i2c_write(0x07);
		i2c_write(0x08);
		i2c_write(0x09);
		i2c_stop();
		
		if(InputState.Button1 == true)
		{
			i2c_start(0xa0);
			i2c_write(0x00);
			i2c_write(0x01);
			i2c_write(0x02);
			i2c_write(0x03);
			i2c_write(0x04);
			i2c_write(0x05);
			i2c_write(0x06);
			i2c_write(0x07);
			i2c_write(0x08);
			i2c_write(0x09);
			i2c_stop();
		}
	}
}
