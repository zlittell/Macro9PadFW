/*
 * NVM_EEPROM.c
 *
 * Created: 10/17/2020 2:24:46 PM
 *  Author: zlitt
 */ 

#include <sam.h>
#include "NVM_EEPROM.h"

// eeprom_page_read
void eeprom_page_read(uint16_t memAddr, uint32_t *readData)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		*(readData + i) = *((uint32_t *)((uint32_t)memAddr) + i);
	}
}

// eeprom_page_verify
// checks values to verify against actual memory locations
// return 1 on failure and 0 on verification
uint8_t eeprom_page_write_verify(uint16_t memAddr, uint32_t *dataToVerify)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		if((*((uint32_t *)((uint32_t)memAddr) + i)) != (*(dataToVerify + i)))
		{
			return 1;
		}
	}
	
	return 0;
}

// eeprom_page_write
// write 64bytes = 32bits * 16 bytes
uint8_t eeprom_page_write(uint16_t memAddr, uint32_t *dataToWrite)
{
	// Copy data to write into page buffer at address 0x00
	for (uint8_t i = 0; i < 16; i++)
	{
		*(pagebuf++) = *(dataToWrite++);
	}
	
	//Load address to write
	NVMCTRL->ADDR.reg = memAddr >> 1;
	
	//Write command
	NVMCTRL->CTRLA.reg = (NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP);
	
	//wait for complete
	while(!(NVMCTRL->INTFLAG.bit.READY))
	{
		if(NVMCTRL->INTFLAG.bit.ERROR)
		{
			return 0;
		}
	}
	
	return 1;
}

// eeprom_row_erase
uint8_t eeprom_row_erase(uint16_t rowAddr)
{
	//Load address of row to erase
	NVMCTRL->ADDR.reg = rowAddr >> 1;
	
	//Issue erase command
	NVMCTRL->CTRLA.reg = (NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER);
	
	//wait for complete
	while(!(NVMCTRL->INTFLAG.bit.READY))
	{
		if(NVMCTRL->INTFLAG.bit.ERROR)
		{
			return 0;
		}
	}
	
	return 1;
}