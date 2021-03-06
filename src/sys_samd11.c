/**
* @file sys_samd11.c
* @brief SAMD11 Specific Code
* @author Zack Littell
* @company Mechanical Squid Factory
* @project Macro9Pad
*/

#include "sam.h"
#include "tusb.h"
#include "macropad.h"

/**
	@brief USB Handler
	@details USB Interrupt Callback from SAMD11
*/
void USB_Handler(void)
{
	tud_int_handler(0);
}

/**
	@brief Configure System Clocks
	@details Configures clocks for a SAMD11
*/
void configureClocks(void)
{
	//_pm_init
	PM->CPUSEL.reg = PM_CPUSEL_CPUDIV_DIV1;
	PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1;
	PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1;
	PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1;
	
	//_sysctrl_init_sources
	SYSCTRL->OSC8M.reg = ((SYSCTRL->OSC8M.reg & SYSCTRL_OSC8M_FRANGE_Msk) |
	(SYSCTRL->OSC8M.reg & SYSCTRL_OSC8M_CALIB_Msk) |
	SYSCTRL_OSC8M_PRESC_0 | SYSCTRL_OSC8M_ENABLE);
	SYSCTRL->OSC32K.reg |= (SYSCTRL_OSC32K_ENABLE | SYSCTRL_OSC32K_EN32K);
	//SYSCTRL->OSCULP32K.reg |= ((SYSCTRL->OSCULP32K.reg & SYSCTRL_OSCULP32K_CALIB_Msk) Not sure why it does this?
	while((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC8MRDY)==0);
	SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ONDEMAND;
	
	//_sysctrl init referenced
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	while((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0);
	SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(1) | SYSCTRL_DFLLMUL_FSTEP(1) | SYSCTRL_DFLLMUL_MUL(48000));
	uint32_t CoarseCALRead =  ((FUSES_DFLL48M_COARSE_CAL_Msk & (*((uint32_t *)FUSES_DFLL48M_COARSE_CAL_ADDR))) >> FUSES_DFLL48M_COARSE_CAL_Pos);
	SYSCTRL->DFLLVAL.reg =  SYSCTRL_DFLLVAL_COARSE(((CoarseCALRead) == 0x3F) ? 0x1F : (CoarseCALRead)) | SYSCTRL_DFLLVAL_FINE(512);
	SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_USBCRM | SYSCTRL_DFLLCTRL_MODE | SYSCTRL_DFLLCTRL_ENABLE);
	while((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0);
	while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
	//SYSCTRL->OSC32K.reg &= (~SYSCTRL_OSC32K_ENABLE);
	
	//_gclk_init_generators last init
	GCLK->GENDIV.reg = (GCLK_GENDIV_DIV(1) | GCLK_GENDIV_ID(0));
	GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC8M | GCLK_GENCTRL_ID(0));
	GCLK->GENDIV.reg = (GCLK_GENDIV_DIV(1) | GCLK_GENDIV_ID(1));
	GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(1));
	GCLK->GENDIV.reg = (GCLK_GENDIV_DIV(1) | GCLK_GENDIV_ID(3));
	GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC32K | GCLK_GENCTRL_ID(3));
	
	//enable PM and clkctrl
	PM->APBBMASK.reg |= (PM_APBBMASK_USB | PM_APBBMASK_NVMCTRL);
	PM->AHBMASK.reg |= (PM_AHBMASK_USB | PM_AHBMASK_NVMCTRL);
	GCLK->CLKCTRL.reg = (USB_GCLK_ID | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_CLKEN);
}

/**
	@brief Initialize Memory
	@details Initializes NVM Flash for storage
*/
void init_Memory(void)
{
	NVMCTRL->CTRLB.reg = (NVMCTRL_CTRLB_CACHEDIS | NVMCTRL_CTRLB_MANW | NVMCTRL_CTRLB_RWS(0));
}

/**
	@brief Initialize TC2
	@details Initialize timer2 for for a ~1mS interrupts
*/
void init_TC2(void)
{
	//Enable TC2 Bus Clock
	PM->APBCMASK.reg |= PM_APBCMASK_TC2;
	
	// Set up Clock Gen
	GCLK->CLKCTRL.reg =
	(GCLK_CLKCTRL_CLKEN |
	GCLK_CLKCTRL_GEN_GCLK3 | GCLK_CLKCTRL_ID_TC1_TC2);
	
	TC2->COUNT16.INTENSET.reg = (TC_INTENSET_ERR | TC_INTENSET_MC0);

	//This should be ~1mS interrupts
	TC2->COUNT16.CC[0].reg = 33;
	
	TC2->COUNT16.CTRLA.reg =
	(TC_CTRLA_PRESCSYNC_PRESC |
	TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_WAVEGEN_NFRQ |
	TC_CTRLA_MODE_COUNT16 | TC_CTRLA_ENABLE);
}

/**
	@brief Initialize IO
	@details Initialize IO for SAMD11 System
*/
void init_IO(void)
{
	//Button1 - PA23
	PORT->Group[0].DIRCLR.reg = PORT_PA23;
	PORT->Group[0].PINCFG[23].reg = PORT_PINCFG_INEN;
	//Button2 - PA02
	PORT->Group[0].DIRCLR.reg = PORT_PA02;
	PORT->Group[0].PINCFG[2].reg = PORT_PINCFG_INEN;
	//Button3 - PA03
	PORT->Group[0].DIRCLR.reg = PORT_PA03;
	PORT->Group[0].PINCFG[3].reg = PORT_PINCFG_INEN;
	//Button4 - PA11
	PORT->Group[0].DIRCLR.reg = PORT_PA11;
	PORT->Group[0].PINCFG[11].reg = PORT_PINCFG_INEN;
	//Button5 - PA06
	PORT->Group[0].DIRCLR.reg = PORT_PA06;
	PORT->Group[0].PINCFG[6].reg = PORT_PINCFG_INEN;
	//Button6 - PA04
	PORT->Group[0].DIRCLR.reg = PORT_PA04;
	PORT->Group[0].PINCFG[4].reg = PORT_PINCFG_INEN;
	//Button7 - PA10
	PORT->Group[0].DIRCLR.reg = PORT_PA10;
	PORT->Group[0].PINCFG[10].reg = PORT_PINCFG_INEN;
	//Button8 - PA07
	PORT->Group[0].DIRCLR.reg = PORT_PA07;
	PORT->Group[0].PINCFG[7].reg = PORT_PINCFG_INEN;
	//Button9 - PA05
	PORT->Group[0].DIRCLR.reg = PORT_PA05;
	PORT->Group[0].PINCFG[5].reg = PORT_PINCFG_INEN;
	//TestIO1 - PA17
	PORT->Group[0].DIRCLR.reg = PORT_PA17;
	PORT->Group[0].PINCFG[17].reg = PORT_PINCFG_INEN;
	//TestIO2 - PA16
	PORT->Group[0].DIRCLR.reg = PORT_PA16;
	PORT->Group[0].PINCFG[16].reg = PORT_PINCFG_INEN;
	//OnboardLED - PA27
	PORT->Group[0].DIRSET.reg = PORT_PA27;
	PORT->Group[0].OUTCLR.reg = PORT_PA27;
	//UART_TX - PA08
	//UART_RX - PA09
	//I2C_SDA - PA14
	//I2C_SCL - PA15
	// USB - PA24
	PORT->Group[0].DIRSET.reg = PORT_PA24;
	PORT->Group[0].OUTCLR.reg = PORT_PA24;
	PORT->Group[0].PINCFG[24].reg = PORT_PINCFG_PMUXEN;
	PORT->Group[0].PMUX[12].reg |= (PORT_PMUX_PMUXE_G);
	//USB - PA25
	PORT->Group[0].DIRSET.reg = PORT_PA25;
	PORT->Group[0].OUTCLR.reg = PORT_PA25;
	PORT->Group[0].PINCFG[25].reg = PORT_PINCFG_PMUXEN;
	PORT->Group[0].PMUX[12].reg |= (PORT_PMUX_PMUXO_G);
}

/**
*	@brief Handle TC2 Interrupt
*	@details Callback for TC2 interrupt.
*/
void TC2_Handler(void)
{
	//If TC2 OverFlow
	if(TC2->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0)
	{
		//Clear and reset interrupt
		TC2->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
		TC2->COUNT16.COUNT.reg = 0;
		
		//Toggle ONBOARD LED
		PORT->Group[0].OUTTGL.reg = PORT_PA27;
		
		//Handle debounce
		Debounce_Handler();
	}
}

/**
*	@brief Enable interrupts
*	@details Enable TC2 and USB interrupts
*/
void enable_interrupts(void)
{
	NVIC_EnableIRQ(TC2_IRQn);
	NVIC_EnableIRQ(USB_IRQn);
}

/**
	@brief Gets a Button
	@details Gets the status of an input
	@param[in] Number of button to get state
	@returns Value of input
*/
uint8_t GetButton(uint8_t buttonToGet)
{
	switch (buttonToGet)
	{
		//Button1 - PA23
		case (1):
		{
			if(PORT->Group[0].IN.reg & PORT_PA23)
			{
				return 1;
			}
			break;
		}
		//Button2 - PA02
		case (2):
		{
			if(PORT->Group[0].IN.reg & PORT_PA02)
			{
				return 1;
			}
			break;
		}
		//Button3 - PA03
		case (3):
		{
			if(PORT->Group[0].IN.reg & PORT_PA03)
			{
				return 1;
			}
			break;
		}
		//Button4 - PA11
		case (4):
		{
			if(PORT->Group[0].IN.reg & PORT_PA11)
			{
				return 1;
			}
			break;
		}
		//Button5 - PA06
		case (5):
		{
			if(PORT->Group[0].IN.reg & PORT_PA06)
			{
				return 1;
			}
			break;
		}
		//Button6 - PA04
		case (6):
		{
			if(PORT->Group[0].IN.reg & PORT_PA04)
			{
				return 1;
			}
			break;
		}
		//Button7 - PA10
		case (7):
		{
			if(PORT->Group[0].IN.reg & PORT_PA10)
			{
				return 1;
			}
			break;
		}
		//Button8 - PA07
		case (8):
		{
			if(PORT->Group[0].IN.reg & PORT_PA07)
			{
				return 1;
			}
			break;
		}
		//Button9 - PA05
		case (9):
		{
			if(PORT->Group[0].IN.reg & PORT_PA05)
			{
				return 1;
			}
			break;
		}
		//TestIO1 - PA17
		case (10):
		{
			if(PORT->Group[0].IN.reg & PORT_PA17)
			{
				return 1;
			}
			break;
		}
		//TestIO2 - PA16
		case (11):
		{
			if(PORT->Group[0].IN.reg & PORT_PA16)
			{
				return 1;
			}
			break;
		}
	}
	
	return 0;
}