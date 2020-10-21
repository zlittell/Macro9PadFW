/*
 * macropad.c
 *
 * Created: 9/24/2020 3:09:56 PM
 *  Author: zlitt
 */ 

#include <stdint.h>
#include "macropad.h"
#include "NVM_EEPROM.h"
#include "NXP_PCA9632.h"

#define PROFILEADDR (LTS_ROW|MEM_PAGE0)
#define DATA_FIRSTVALUE_POS 24
#define DATA_SECONDVALUE_POS 16
#define DATA_THIRDVALUE_POS  8
#define DATA_SHIFTEDMASK 0xFF

struct DeviceInputs InputState = {0};

struct DeviceProfile MacropadProfile;
struct CommandBufferStruct CMDBuffer;
uint32_t profileMemory[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t memoryWriteCount = 0;

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
	memoryWriteCount++;
	
	//determine if should use save 1 or 2
	uint8_t arrayCursor = 0;
	if ((uint16_t)((profileMemory[arrayCursor+5] >> DATA_FIRSTVALUE_POS)|((profileMemory[arrayCursor+5] >> DATA_THIRDVALUE_POS)&0xFF00)) > 0x4E20)
	{
		//use page 2 add size of save to cursor
		arrayCursor += 6;
	}
		
	profileMemory[arrayCursor] = 
		((MacropadProfile.profileLED.Red)|
		(MacropadProfile.profileLED.Green<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.profileLED.Blue<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.profileLED.Brightness<<DATA_FIRSTVALUE_POS));
	arrayCursor++;
		
	profileMemory[arrayCursor] = 
		((MacropadProfile.Button1.Modifier)|
		(MacropadProfile.Button1.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button2.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button2.Button<<DATA_FIRSTVALUE_POS));
	arrayCursor++;
	
	profileMemory[arrayCursor] =
		((MacropadProfile.Button3.Modifier)|
		(MacropadProfile.Button3.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button4.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button4.Button<<DATA_FIRSTVALUE_POS));
	arrayCursor++;
		
	profileMemory[arrayCursor] =
		((MacropadProfile.Button5.Modifier)|
		(MacropadProfile.Button5.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button6.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button6.Button<<DATA_FIRSTVALUE_POS));
	arrayCursor++;
		
	profileMemory[arrayCursor] =
		((MacropadProfile.Button7.Modifier)|
		(MacropadProfile.Button7.Button<<DATA_THIRDVALUE_POS)|
		(MacropadProfile.Button8.Modifier<<DATA_SECONDVALUE_POS)|
		(MacropadProfile.Button8.Button<<DATA_FIRSTVALUE_POS));
	arrayCursor++;
	/*
	profileMemory[arrayCursor] =
	((MacropadProfile.Button9.Modifier)|
	(MacropadProfile.Button9.Button<<DATA_THIRDVALUE_POS)|
		(((uint32_t)memoryWriteCount<<8)&0x00FF0000)|
		((uint32_t)memoryWriteCount<<24));
		*/
	profileMemory[arrayCursor] =
	((MacropadProfile.Button9.Modifier)|
	(MacropadProfile.Button9.Button<<DATA_THIRDVALUE_POS)|
		((memoryWriteCount<<8)&0x00FF0000)|
		(memoryWriteCount<<24));
		
	eeprom_row_erase(PROFILEADDR);
	eeprom_page_write(PROFILEADDR, profileMemory);
	LoadProfile();	//reload to confirm save and to select correct save 1 or 2
}

void LoadProfile(void)
{
	eeprom_page_read(PROFILEADDR, profileMemory);
	uint8_t arrayCursor = 0;
	
	//determine if should use save 1 or 2
	if ((uint16_t)((profileMemory[arrayCursor+5] >> DATA_FIRSTVALUE_POS)|((profileMemory[arrayCursor+5] >> DATA_THIRDVALUE_POS)&0xFF00)) > 0x4E20)
	{
		//use page 2 add size of save to cursor
		arrayCursor += 6;
	}
	
	// 1st 32bits
	MacropadProfile.profileLED.Red = ((uint8_t)(profileMemory[arrayCursor]&DATA_SHIFTEDMASK));
	MacropadProfile.profileLED.Green = ((uint8_t)((profileMemory[arrayCursor]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.profileLED.Blue = ((uint8_t)((profileMemory[arrayCursor]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.profileLED.Brightness = ((uint8_t)(profileMemory[arrayCursor]>>DATA_FIRSTVALUE_POS));
	arrayCursor++;
	
	// 2nd 32bits
	MacropadProfile.Button1.Modifier = ((uint8_t)(profileMemory[arrayCursor]&DATA_SHIFTEDMASK));
	MacropadProfile.Button1.Button = ((uint8_t)((profileMemory[arrayCursor]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button2.Modifier = ((uint8_t)((profileMemory[arrayCursor]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button2.Button = ((uint8_t)(profileMemory[arrayCursor]>>DATA_FIRSTVALUE_POS));
	arrayCursor++;
	
	// 3rd 32bits
	MacropadProfile.Button3.Modifier = ((uint8_t)(profileMemory[arrayCursor]&DATA_SHIFTEDMASK));
	MacropadProfile.Button3.Button = ((uint8_t)((profileMemory[arrayCursor]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button4.Modifier = ((uint8_t)((profileMemory[arrayCursor]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button4.Button = ((uint8_t)(profileMemory[arrayCursor]>>DATA_FIRSTVALUE_POS));
	arrayCursor++;
	
	// 4th 32bits
	MacropadProfile.Button5.Modifier = ((uint8_t)(profileMemory[arrayCursor]&DATA_SHIFTEDMASK));
	MacropadProfile.Button5.Button = ((uint8_t)((profileMemory[arrayCursor]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button6.Modifier = ((uint8_t)((profileMemory[arrayCursor]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button6.Button = ((uint8_t)(profileMemory[arrayCursor]>>DATA_FIRSTVALUE_POS));
	arrayCursor++;
	
	// 5th 32bits
	MacropadProfile.Button7.Modifier = ((uint8_t)(profileMemory[arrayCursor]&DATA_SHIFTEDMASK));
	MacropadProfile.Button7.Button = ((uint8_t)((profileMemory[arrayCursor]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button8.Modifier = ((uint8_t)((profileMemory[arrayCursor]>>DATA_SECONDVALUE_POS)&DATA_SHIFTEDMASK));
	MacropadProfile.Button8.Button = ((uint8_t)(profileMemory[arrayCursor]>>DATA_FIRSTVALUE_POS));
	arrayCursor++;
	
	// 6th 32bits
	MacropadProfile.Button9.Modifier = ((uint8_t)(profileMemory[arrayCursor]&DATA_SHIFTEDMASK));
	MacropadProfile.Button9.Button = ((uint8_t)((profileMemory[arrayCursor]>>DATA_THIRDVALUE_POS)&DATA_SHIFTEDMASK));
	memoryWriteCount = ((profileMemory[arrayCursor] >> DATA_FIRSTVALUE_POS)|((profileMemory[arrayCursor] >> DATA_THIRDVALUE_POS)&0xFF00));
	
	LED_updateRGB(MacropadProfile.profileLED.Blue, MacropadProfile.profileLED.Green, MacropadProfile.profileLED.Red);
	LED_updateBrightness(MacropadProfile.profileLED.Brightness);
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
