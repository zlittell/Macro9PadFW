/*
 * main.h
 *
 * Created: 9/17/2020 3:49:35 PM
 *  Author: zlitt
 */ 


#ifndef MAIN_H_
#define MAIN_H_

// this is a sample profile programmed initially
uint8_t __attribute__ ((section (".deviceProfileSection"))) initProfile[64] = 
{
	//Default Save 1
	// 32bytes 1
	0x11,	// RGB RED
	0x22,	// RGB Green
	0x33,	// RGB Blue
	0xFF,	// RGB Brightness
	// 32bytes 2
	0x02,	// Button1 LShift
	0x1d,	// Button1 z
	0x00,	// Button2 noMod
	0x04,	// Button2 a
	// 32bytes 3
	0x00,	// Button3 noMod
	0x06,	// Button3 c
	0x00,	// Button4 noMod
	0x0e,	// Button4 k
	// 32bytes 4
	0x02,	// Button5 LShift
	0x1e,	// Button5 l
	0x00,	// Button6 noMod
	0x10,	// Button6 m
	// 32bytes 5
	0x00,	// Button7 noMod
	0x11,	// Button7 n
	0x00,	// Button8 noMod
	0x12,	// Button8 o
	// 32bytes 6
	0x00,	// Button9 noMod
	0x13,	// Button9 p
	0x00,	// WriteCounterA
	0x00,	// WriteCounterB
	// Filler for Save 2
	// 32bytes 7
	0x00,0x00,0x00,0x00,
	// 32bytes 8
	0x00,0x00,0x00,0x00,
	// 32bytes 9
	0x00,0x00,0x00,0x00,
	// 32bytes 10
	0x00,0x00,0x00,0x00,
	// 32bytes 11
	0x00,0x00,0x00,0x00,
	// 32bytes 12
	0x00,0x00,0x00,0x00,
	// 32bytes 13
	0x00,0x00,0x00,0x00,
	// 32bytes 14	
	0x00,0x00,0x00,0x00,
	// 32bytes 15
	0x00,0x00,0x00,0x00,
	// 32bytes 16
	0x00,0x00,0x00,0x00
};


#endif /* MAIN_H_ */