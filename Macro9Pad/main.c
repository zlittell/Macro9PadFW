/*
 * Macro9Pad.c
 *
 * Created: 9/15/2020 1:43:22 PM
 * Author : zlitt
 */ 

#define CFG_TUSB_MCU OPT_MCU_SAMD11
#define CFG_TUSB_DEBUG 2 //TEMP


#include "sam.h"
#include "system_configuration.h"
#include "tusb.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	  //Set up clock outputs
	  //clock 0 8MHz
	  PORT->Group[0].DIRSET.reg = PORT_PA08;
	  PORT->Group[0].PINCFG[8].reg |= PORT_PINCFG_PMUXEN;
	  //PORT->Group[0].PMUX[4].reg = PORT_PMUX_PMUXE_H;
	  //clock1 48MHz
	  PORT->Group[0].DIRSET.reg = PORT_PA09;
	  PORT->Group[0].PINCFG[9].reg = PORT_PINCFG_PMUXEN;
	  PORT->Group[0].PMUX[4].reg = (PORT_PMUX_PMUXE_H | PORT_PMUX_PMUXO_H);
	  //clock 3 should be 32khz
	  PORT->Group[0].DIRSET.reg = PORT_PA17;
	  PORT->Group[0].PINCFG[17].reg |= PORT_PINCFG_PMUXEN;
	  PORT->Group[0].PMUX[8].reg = PORT_PMUX_PMUXO_H;
	  
	  configureClocks();
	  init_IO();
	  //init_TC2();

	  //enable_interrupts();
	  NVIC_EnableIRQ(USB_IRQn);
	  
	  tusb_init();
	  
	  PORT->Group[0].DIRSET.reg = PORT_PA16;
	  PORT->Group[0].OUTCLR.reg = PORT_PA16;

    /* Replace with your application code */
    while (1) 
    {
		__NOP();
		tud_task();
    }
}
