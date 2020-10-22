/**
* @file Macro9Pad.c
* @brief Macropad Interface Logic
* @author Zack Littell
* @company Mechanical Squid Factory
* @project Macro9Pad
*/

#include <stdint.h>
#include "sys_samd11.h"
#include "macropad.h"
#include "MSF_NVM_FLASH/MSF_NVM_FLASH.h"
#include "MSF_RGB/MSF_RGB.h"

/************************************************************************/
/* File Defines                                                         */
/************************************************************************/
#define PROFILEADDR (0x3F00)
#define DATA_FIRSTVALUE_POS 24
#define DATA_SECONDVALUE_POS 16
#define DATA_THIRDVALUE_POS  8
#define DATA_SHIFTEDMASK 0xFF
#define SWITCHDEBOUNCE 5

/************************************************************************/
/* Global Variables                                                     */
/************************************************************************/
struct DeviceInputs InputState = {0};
struct DeviceProfile MacropadProfile;
struct CommandBufferStruct CMDBuffer;
uint32_t profileMemory[16] = {0};
uint16_t memoryWriteCount = 0;

/************************************************************************/
/* Code                                                                 */
/************************************************************************/
enum{
	RGB_Blue=0,
	RGB_Green,
	RGB_Red,
	RGB_PacketSize
};

/**
	@brief Update device LEDs
	@details Updates RGB values and brightness for display.
*/
static void UpdateLED(void)
{
	uint8_t values[RGB_PacketSize] = {0};
	values[RGB_Red] = MacropadProfile.profileLED.Red;
	values[RGB_Green] = MacropadProfile.profileLED.Green;
	values[RGB_Blue] = MacropadProfile.profileLED.Blue;
	
	LED_UpdateAll(LEDADDR, values, RGB_PacketSize);
	LED_UpdateBrightness(LEDADDR, MacropadProfile.profileLED.Brightness);
}

/**
	@brief Sleep device
	@details puts device into a sleep mode
*/
void SleepDevice(void)
{
	LED_Disable(LEDADDR);
}

/**
	@brief Wake device
	@details wakes device from sleep
*/
void WakeDevice(void)
{
	LED_Enable(LEDADDR);
}

/**
*	@brief Handle button debounce.
*	@details Call on timer interrupt to process inputs while preventing bounce.
*/
void Debounce_Handler(void)
{
	static struct DeviceInput_Debounce InputDebounceCount = {0};
	
	if(GetButton(1))
	{
		if (InputDebounceCount.Button1 >= SWITCHDEBOUNCE)
		{
			InputState.Button1 = true;
		}
		else
		{
			InputDebounceCount.Button1++;
		}
	}
	else
	{
		InputDebounceCount.Button1 = 0;
		InputState.Button1 = false;
	}
	
	
	if(GetButton(2))
	{
		if (InputDebounceCount.Button2 >= SWITCHDEBOUNCE)
		{
			InputState.Button2 = true;
		}
		else
		{
			InputDebounceCount.Button2++;
		}
	}
	else
	{
		InputDebounceCount.Button2 = 0;
		InputState.Button2 = false;
	}
	
	
	if(GetButton(3))
	{
		if (InputDebounceCount.Button3 >= SWITCHDEBOUNCE)
		{
			InputState.Button3 = true;
		}
		else
		{
			InputDebounceCount.Button3++;
		}
	}
	else
	{
		InputDebounceCount.Button3 = 0;
		InputState.Button3 = false;
	}
	
	
	if(GetButton(4))
	{
		if (InputDebounceCount.Button4 >= SWITCHDEBOUNCE)
		{
			InputState.Button4 = true;
		}
		else
		{
			InputDebounceCount.Button4++;
		}
	}
	else
	{
		InputDebounceCount.Button4 = 0;
		InputState.Button4 = false;
	}
	
	
	if(GetButton(5))
	{
		if (InputDebounceCount.Button5 >= SWITCHDEBOUNCE)
		{
			InputState.Button5 = true;
		}
		else
		{
			InputDebounceCount.Button5++;
		}
	}
	else
	{
		InputDebounceCount.Button5 = 0;
		InputState.Button5 = false;
	}
	
	
	if(GetButton(6))
	{
		if (InputDebounceCount.Button6 >= SWITCHDEBOUNCE)
		{
			InputState.Button6 = true;
		}
		else
		{
			InputDebounceCount.Button6++;
		}
	}
	else
	{
		InputDebounceCount.Button6 = 0;
		InputState.Button6 = false;
	}
	
	
	if(GetButton(7))
	{
		if (InputDebounceCount.Button7 >= SWITCHDEBOUNCE)
		{
			InputState.Button7 = true;
		}
		else
		{
			InputDebounceCount.Button7++;
		}
	}
	else
	{
		InputDebounceCount.Button7 = 0;
		InputState.Button7 = false;
	}
	
	
	if(GetButton(8))
	{
		if (InputDebounceCount.Button8 >= SWITCHDEBOUNCE)
		{
			InputState.Button8 = true;
		}
		else
		{
			InputDebounceCount.Button8++;
		}
	}
	else
	{
		InputDebounceCount.Button8 = 0;
		InputState.Button8 = false;
	}
	
	
	if(GetButton(9))
	{
		if (InputDebounceCount.Button9 >= SWITCHDEBOUNCE)
		{
			InputState.Button9 = true;
		}
		else
		{
			InputDebounceCount.Button9++;
		}
	}
	else
	{
		InputDebounceCount.Button9 = 0;
		InputState.Button9 = false;
	}
	
	
	if(GetButton(10))
	{
		if (InputDebounceCount.TestIO1 >= SWITCHDEBOUNCE)
		{
			InputState.TestIO1 = true;
		}
		else
		{
			InputDebounceCount.TestIO1++;
		}
	}
	else
	{
		InputDebounceCount.TestIO1 = 0;
		InputState.TestIO1 = false;
	}
	
	
	if(GetButton(11))
	{
		if (InputDebounceCount.TestIO2 >= SWITCHDEBOUNCE)
		{
			InputState.TestIO2 = true;
		}
		else
		{
			InputDebounceCount.TestIO2++;
		}
	}
	else
	{
		InputDebounceCount.TestIO2 = 0;
		InputState.TestIO2 = false;
	}
}

/**
	@brief Parse profile message
	@detail Takes a profile message and moves it into device profile
	@param[in] message The profile message to be parsed
	@param[in] len Length of the message to be parsed
	@returns Length of message parsed or 0 on failure
*/
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

/**
	@brief Copy profile to buffer
	@details Copies the current device profile into an array
	@param[out] buffer Array for holding the copied profile
*/
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

/**
	@brief Save profile
	@details Saves the current profile to flash memory
*/
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

	profileMemory[arrayCursor] =
	((MacropadProfile.Button9.Modifier)|
	(MacropadProfile.Button9.Button<<DATA_THIRDVALUE_POS)|
		((memoryWriteCount<<8)&0x00FF0000)|
		(memoryWriteCount<<24));
		
	flash_row_erase(PROFILEADDR);
	flash_page_write(PROFILEADDR, profileMemory);
	LoadProfile();	//reload to confirm save and to select correct save 1 or 2
}

/**
	@brief Load Profile
	@details Loads profile from flash memory
*/
void LoadProfile(void)
{
	flash_page_read(PROFILEADDR, profileMemory);
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
	
	UpdateLED();
}

/**
	@brief Process Inputs
	@details Processes input structure
	@param[out] report Report to fill with keystrokes
	@returns Whether or not to send a packet (1=send)
*/
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

/**
	@brief Command Add
	@details Adds a command to the command buffer
	@param[in] command Command to add to command buffer
*/
void CommandBufferAdd(uint8_t command)
{
	CMDBuffer.buffer[CMDBuffer.wP] = command;
	
	if(CMDBuffer.wP >= (COMMANDBUFFERSIZE-1)){CMDBuffer.wP = 0;}
	else {CMDBuffer.wP++;}
}

/**
	@brief Command Buffer Process
	@details Processes the command buffer
	@returns Spits out the next command in the command ring buffer, 0x00 if nothing in buffer.
*/
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

/**
	@brief Command Parse
	@details Parses commands received on the USB config interface
	@param[in] message
	@param[in] len length of the message to parse
*/
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