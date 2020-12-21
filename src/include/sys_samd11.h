/*
 * samd11.h
 *
 * Created: 10/21/2020 3:15:19 PM
 *  Author: zlitt
 */ 


#ifndef INIT_SAMD11_H_
#define INIT_SAMD11_H_


void configureClocks(void);
void init_TC2(void);
void init_IO(void);
void init_Memory(void);
void enable_interrupts(void);
uint8_t GetButton(uint8_t);


#endif /* INIT_SAMD11_H_ */