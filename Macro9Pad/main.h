/*
 * main.h
 *
 * Created: 9/17/2020 3:49:35 PM
 *  Author: zlitt
 */ 


#ifndef MAIN_H_
#define MAIN_H_

// this is a sample profile programmed initially
uint8_t __attribute__ ((section (".deviceProfileSection"))) initProfile[22] = 
{
	0xFF,	// RGB RED
	0x00,	// RGB Green
	0xFF,	// RGB Blue
	0xFF,	// RGB Brightness
	0x02,	// Button1 LShift
	0x1d,	// Button1 z
	0x00,	// Button2 noMod
	0x04,	// Button2 a
	0x00,	// Button3 noMod
	0x06,	// Button3 c
	0x00,	// Button4 noMod
	0x0e,	// Button4 k
	0x02,	// Button5 LShift
	0x1e,	// Button5 l
	0x00,	// Button6 noMod
	0x10,	// Button6 m
	0x00,	// Button7 noMod
	0x11,	// Button7 n
	0x00,	// Button8 noMod
	0x12,	// Button8 o
	0x00,	// Button9 noMod
	0x13	// Button9 p
};


#endif /* MAIN_H_ */