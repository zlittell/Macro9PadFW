/*
 * macropad.c
 *
 * Created: 9/24/2020 3:09:56 PM
 *  Author: zlitt
 */ 

#include "macropad.h"
#include "SystemStructures.h"

struct DeviceProfile MacropadProfile = 
{
	.profileLED.Red = 0x00;
	.profileLED.Green = 0x00;
	.profileLED.Blue = 0x00;
	.profileLED.Brightness = 0x00;
	.Button1.Modifier = 0x00;
	.Button1.Button = 0x00;
	.Button2.Modifier = 0x00;
	.Button2.Button = 0x00;
	.Button3.Modifier = 0x00;
	.Button3.Button = 0x00;
	.Button4.Modifier = 0x00;
	.Button4.Button = 0x00;
	.Button5.Modifier = 0x00;
	.Button5.Button = 0x00;
	.Button6.Modifier = 0x00;
	.Button6.Button = 0x00;
	.Button7.Modifier = 0x00;
	.Button7.Button = 0x00;
	.Button8.Modifier = 0x00;
	.Button8.Button = 0x00;
	.Button9.Modifier = 0x00;
	.Button9.Button = 0x00;
};

//update a profile
void ParseProfileMessage(uint8_t *message, uint8_t len)
{
	
}

//save the profile to eeprom

//read the profile from eeprom

//This can be deleted in production
void fillTestProfile(void)
{
	MacropadProfile.profileLED.Red = 0xFF;
	MacropadProfile.profileLED.Blue = 0xFF;
	MacropadProfile.profileLED.Brightness = 0xFF;
	MacropadProfile.Button1.Modifier = 0x02;	//Shift
	MacropadProfile.Button1.Button = 0x1d; //z
	MacropadProfile.Button2.Button = 0x04; //a
	MacropadProfile.Button3.Button = 0x06; //c
	MacropadProfile.Button4.Button = 0x0e; //k
	MacropadProfile.Button5.Modifier = 0x02; //Shift
	MacropadProfile.Button5.Button = 0x1e; //1
	MacropadProfile.Button6.Button = 0x10; //m
	MacropadProfile.Button7.Button = 0x11; //n
	MacropadProfile.Button8.Button = 0x12; //o
	MacropadProfile.Button9.Button = 0x13; //p
}

