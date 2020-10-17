/*
 * NVM_EEPROM.h
 *
 * Created: 10/17/2020 2:24:59 PM
 *  Author: zlitt
 */ 


#ifndef NVM_EEPROM_H_
#define NVM_EEPROM_H_

#define LTS_ROW 0x3F00
#define MEM_PAGE0 0x00
#define MEM_PAGE1 0x40
#define MEM_PAGE2 0x80
#define MEM_PAGE3 0xC0

void eeprom_page_read(uint16_t, uint32_t*);
uint8_t eeprom_page_write_verify(uint16_t, uint32_t*);
uint8_t eeprom_page_write(uint16_t, uint32_t*);
uint8_t eeprom_row_erase(uint16_t);


volatile uint32_t *pagebuf;



#endif /* NVM_EEPROM_H_ */