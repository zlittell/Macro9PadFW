/*
 * macropad.c
 *
 * Created: 9/24/2020 3:09:56 PM
 *  Author: zlitt
 */ 

#include <stdint.h>
#include "macropad.h"
#include "SystemStructures.h"

/*
 * RGB - 4 (Red, Green, Blue, Brightness)
 * B1  - 2
 * B2  - 2
 * B3  - 2
 * B4  - 2
 * B5  - 2
 * B6  - 2
 * B7  - 2
 * B8  - 2
 * B9  - 2
 * --------
 *       22 bytes
*/
#define PROFILE_MESSAGE_LENGTH 22

struct DeviceProfile MacropadProfile;

//update a profile
uint8_t ParseProfileMessage(uint8_t *message, uint8_t len)
{
	if(len == PROFILE_MESSAGE_LENGTH)
	{
		uint8_t *aP = &MacropadProfile.profileLED.Red;
		
		for (uint8_t i = 0; i < len; i++)
		{
			*aP = *message;
			aP++;
			message++;
		}		
		return len;
	}
	
	return 0;
}

//save the profile to eeprom

//read the profile from eeprom

void commandParse(uint8_t *message, uint8_t len)
{
	
}

//This can be deleted in production
void fillTestProfile(void)
{
	MacropadProfile.profileLED.Red = 0xFF;
	MacropadProfile.profileLED.Blue = 0xFF;
	MacropadProfile.profileLED.Brightness = 0xFF;
	MacropadProfile.Button1.Modifier = MODIFIER_LEFTSHIFT;
	MacropadProfile.Button1.Button = 0x1d; //z
	MacropadProfile.Button2.Button = 0x04; //a
	MacropadProfile.Button3.Button = 0x06; //c
	MacropadProfile.Button4.Button = 0x0e; //k
	MacropadProfile.Button5.Modifier = MODIFIER_LEFTSHIFT;
	MacropadProfile.Button5.Button = 0x1e; //1
	MacropadProfile.Button6.Button = 0x10; //m
	MacropadProfile.Button7.Button = 0x11; //n
	MacropadProfile.Button8.Button = 0x12; //o
	MacropadProfile.Button9.Button = 0x13; //p
}

