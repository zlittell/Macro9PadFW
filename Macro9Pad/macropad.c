/*
 * macropad.c
 *
 * Created: 9/24/2020 3:09:56 PM
 *  Author: zlitt
 */ 

#include <stdint.h>
#include "macropad.h"
#include "SystemStructures.h"

extern DeviceInputs InputState;
extern DeviceProfile MacropadProfile;

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

uint8_t ProcessInputs(MacroPad_KeyboardReport *report)
{
	static union KBStateTrack state = {0};
	state.BITS.CLEANUP = 0;
		
	/*------------- Button1 -------------*/
	if (InputState.Button1)
	{
		report->modifier |= MacropadProfile.Button1.Modifier;
		report->buttons[0] = MacropadProfile.Button1.Button;
		state.BITS.B1 = 1;
	}
	else
	{
		if(state.BITS.B1)
		{
			state.BITS.B1 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button2 -------------*/
	if (InputState.Button2)
	{
		report->modifier |= MacropadProfile.Button2.Modifier;
		report->buttons[1] = MacropadProfile.Button2.Button;
		state.BITS.B2 = 1;
	}
	else
	{
		if(state.BITS.B2)
		{
			state.BITS.B2 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button3 -------------*/
	if (InputState.Button3)
	{
		report->modifier |= MacropadProfile.Button3.Modifier;
		report->buttons[2] = MacropadProfile.Button3.Button;
		state.BITS.B3 = 1;
	}
	else
	{
		if(state.BITS.B3)
		{
			state.BITS.B3 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button4 -------------*/
	if (InputState.Button4)
	{
		report->modifier |= MacropadProfile.Button4.Modifier;
		report->buttons[3] = MacropadProfile.Button4.Button;
		state.BITS.B4 = 1;
	}
	else
	{
		if(state.BITS.B4)
		{
			state.BITS.B4 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button5 -------------*/
	if (InputState.Button5)
	{
		report->modifier |= MacropadProfile.Button5.Modifier;
		report->buttons[4] = MacropadProfile.Button5.Button;
		state.BITS.B5 = 1;
	}
	else
	{
		if(state.BITS.B5)
		{
			state.BITS.B5 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button6 -------------*/
	if (InputState.Button6)
	{
		report->modifier |= MacropadProfile.Button6.Modifier;
		report->buttons[5] = MacropadProfile.Button6.Button;
		state.BITS.B6 = 1;
	}
	else
	{
		if(state.BITS.B6)
		{
			state.BITS.B6 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button7 -------------*/
	if (InputState.Button7)
	{
		report->modifier |= MacropadProfile.Button7.Modifier;
		report->buttons[6] = MacropadProfile.Button7.Button;
		state.BITS.B7 = 1;		
	}
	else
	{
		if(state.BITS.B7)
		{
			state.BITS.B7 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button8 -------------*/
	if (InputState.Button8)
	{
		report->modifier |= MacropadProfile.Button8.Modifier;
		report->buttons[7] = MacropadProfile.Button8.Button;
		state.BITS.B8 = 1;
	}
	else
	{
		if(state.BITS.B8)
		{
			state.BITS.B8 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	/*------------- Button9 -------------*/
	if (InputState.Button9)
	{
		report->modifier |= MacropadProfile.Button9.Modifier;
		report->buttons[8] = MacropadProfile.Button9.Button;
		state.BITS.B9 = 1;
	}
	else
	{
		if(state.BITS.B9)
		{
			state.BITS.B9 = 0;
			state.BITS.CLEANUP = 1;
		}
	}
	
	//Check state and return 1 or 0 (send packet or send nothing)
	if (state.STATE){return 1;}
	else {return 0;}
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

