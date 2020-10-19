/*
 * macropad.c
 *
 * Created: 9/24/2020 3:09:56 PM
 *  Author: zlitt
 */ 

#include <stdint.h>
#include "macropad.h"
#include "NVM_EEPROM.h"
#include "SystemStructures.h"

#define PROFILEADDR (LTS_ROW|MEM_PAGE0)
#define DATA_FIRSTVALUE_POS 24
#define DATA_SECONDVALUE_POS 16
#define DATA_THIRDVALUE_POS  8
#define DATA_SHIFTEDMASK 0xFF

extern DeviceInputs InputState;
struct DeviceProfile MacropadProfile;
struct CommandBufferStruct CMDBuffer;

//update a profile
uint8_t ParseProfileMessage(uint8_t const *message, uint8_t const len)
{
	if(len == PROFILE_MESSAGE_LENGTH)
	{
		uint8_t *aP = &MacropadProfile.profileLED.Red;
		message++;	// move past CMD byte
		
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

void CopyProfileToBuffer(uint8_t *buffer)
{
	uint8_t *aP = &MacropadProfile.profileLED.Red;
	*buffer = CMD_SendProfile;
	buffer++;
	
	for(uint8_t i = 0; i < PROFILE_MESSAGE_LENGTH; i++)
	{
		*buffer = *aP;
		aP++;
		buffer++;
	}
}

void SaveProfile(void)
{
	// @todo save to eeprom
	uint32_t data[16] = {0xFFFFFFFF};
		
	data[0] = 
		((MacropadProfile.profileLED.Red)|
		(MacropadProfile.profileLED.Green<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.profileLED.Blue<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.profileLED.Brightness<<DATA_FIRSTVALUE_POS));
		
	data[1] = 
		((MacropadProfile.Button1.Modifier)|
		(MacropadProfile.Button1.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button2.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button2.Button<<DATA_FIRSTVALUE_POS));
	
	data[2] =
		((MacropadProfile.Button3.Modifier)|
		(MacropadProfile.Button3.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button4.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button4.Button<<DATA_FIRSTVALUE_POS));
		
	data[3] =
		((MacropadProfile.Button5.Modifier)|
		(MacropadProfile.Button5.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button6.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button6.Button<<DATA_FIRSTVALUE_POS));
		
	data[4] =
		((MacropadProfile.Button7.Modifier)|
		(MacropadProfile.Button7.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button8.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button8.Button<<DATA_FIRSTVALUE_POS));
	
	data[5] =
		((MacropadProfile.Button9.Modifier)|
		(MacropadProfile.Button9.Button<<DATA_THIRDVALUE_POS)|
		(0xFFFF0000));
		
	eeprom_row_erase(PROFILEADDR);
	eeprom_page_write(PROFILEADDR, data);
}

void LoadProfile(void)
{
	uint32_t data[16] = {0};
	eeprom_page_read(PROFILEADDR, data);
	
	// 1st 32bits
	MacropadProfile.profileLED.Red = ((uint8_t)(data[0]&DATA_SHIFTEDMASK));
	MacropadProfile.profileLED.Green = ((uint8_t)((data[0]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.profileLED.Blue = ((uint8_t)((data[0]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.profileLED.Brightness = ((uint8_t)(data[0]>>DATA_FIRSTVALUE_POS));
	
	// 2nd 32bits
	MacropadProfile.Button1.Modifier = ((uint8_t)(data[1]&DATA_SHIFTEDMASK));
	MacropadProfile.Button1.Button = ((uint8_t)((data[1]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button2.Modifier = ((uint8_t)((data[1]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button2.Button = ((uint8_t)(data[1]>>DATA_FIRSTVALUE_POS));
	
	// 3rd 32bits
	MacropadProfile.Button3.Modifier = ((uint8_t)(data[2]&DATA_SHIFTEDMASK));
	MacropadProfile.Button3.Button = ((uint8_t)((data[2]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button4.Modifier = ((uint8_t)((data[2]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button4.Button = ((uint8_t)(data[2]>>DATA_FIRSTVALUE_POS));
	
	// 4th 32bits
	MacropadProfile.Button5.Modifier = ((uint8_t)(data[3]&DATA_SHIFTEDMASK));
	MacropadProfile.Button5.Button = ((uint8_t)((data[3]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button6.Modifier = ((uint8_t)((data[3]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button6.Button = ((uint8_t)(data[3]>>DATA_FIRSTVALUE_POS));
	
	// 5th 32bits
	MacropadProfile.Button7.Modifier = ((uint8_t)(data[4]&DATA_SHIFTEDMASK));
	MacropadProfile.Button7.Button = ((uint8_t)((data[4]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button8.Modifier = ((uint8_t)((data[4]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button8.Button = ((uint8_t)(data[4]>>DATA_FIRSTVALUE_POS));
	
	// 6th 32bits
	MacropadProfile.Button9.Modifier = ((uint8_t)(data[5]&DATA_SHIFTEDMASK));
	MacropadProfile.Button9.Button = ((uint8_t)((data[5]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
}

void CommandBufferAdd(uint8_t command)
{
	CMDBuffer.buffer[CMDBuffer.wP] = command;
	
	if(CMDBuffer.wP >= (COMMANDBUFFERSIZE-1)){CMDBuffer.wP = 0;}
	else {CMDBuffer.wP++;}
}

uint8_t CommandBufferProcess(void)
{
	if(CMDBuffer.rP != CMDBuffer.wP)
	{
		uint8_t readCommand = CMDBuffer.buffer[CMDBuffer.rP];
		
		if(CMDBuffer.rP >= (COMMANDBUFFERSIZE-1)){CMDBuffer.rP = 0;}
		else {CMDBuffer.rP++;}
		
		return readCommand;
	}
	
	return 0;
}

void CommandParse(uint8_t const *message, uint8_t const len)
{
	switch(*message)
	{
		case (CMD_ReceiveProfile):
		{
			ParseProfileMessage(message, len);
			break;
		}
		case (CMD_SendProfile):
		{
			//GlobalCommandFlags.bits.CMDFLAG_SendProfile = 1;
			CommandBufferAdd(*message);
			break;
		}
		case (CMD_SaveProfile):
		{
			SaveProfile();
			break;
		}
	}
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

void test(void)
{
	if(MacropadProfile.Button1.Button!=0x1e)
	{
		MacropadProfile.Button1.Button = 0x1e;
		MacropadProfile.Button2.Button = 0x10;
		MacropadProfile.Button3.Button = 0x11;
		MacropadProfile.Button4.Button = 0x12;
		MacropadProfile.Button5.Button = 0x13;
		MacropadProfile.Button6.Button = 0x1d;
		MacropadProfile.Button7.Button = 0x04;
		MacropadProfile.Button8.Button = 0x06;
		MacropadProfile.Button9.Button = 0x0e;
		SaveProfile();
	}
	else
	{
		MacropadProfile.Button2.Button++;
		SaveProfile();
	}
}
