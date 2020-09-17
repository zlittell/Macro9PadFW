/*
 * SystemStructures.h
 *
 * Created: 9/17/2020 2:43:29 PM
 *  Author: zlitt
 */ 
#ifndef SYSTEMSTRUCTURES_H_
#define SYSTEMSTRUCTURES_H_

#include <stdbool.h>

struct DeviceInputs
{
	bool Button1;
	bool Button2;
	bool Button3;
	bool Button4;
	bool Button5;
	bool Button6;
	bool Button7;
	bool Button8;
	bool Button9;
	bool TestIO1;
	bool TestIO2 ;
};

struct DeviceInput_Debounce
{
	uint8_t Button1;
	uint8_t Button2;
	uint8_t Button3;
	uint8_t Button4;
	uint8_t Button5;
	uint8_t Button6;
	uint8_t Button7;
	uint8_t Button8;
	uint8_t Button9;
	uint8_t TestIO1;
	uint8_t TestIO2;
};


#endif /* SYSTEMSTRUCTURES_H_ */