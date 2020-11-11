/*
 * macropad.h
 *
 * Created: 9/24/2020 3:10:10 PM
 *  Author: zlitt
 */ 


#ifndef MACROPAD_H_
#define MACROPAD_H_

#include <stdbool.h>

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
// Add 1 Command Byte
#define USB_MESSAGE_LENGTH PROFILE_MESSAGE_LENGTH+1

#define LEDADDR 0x60

typedef struct DeviceInputs
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
}DeviceInputs;

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

struct CommandStatus
{
	uint8_t SendProfile : 1;
	uint8_t SaveProfile : 1;
};

enum Commands
{
	CMD_ReceiveProfile = 0xA0,
	CMD_SendProfile = 0xB0,
	CMD_SaveProfile = 0xB1,
	CMD_Bootloader = 0xC0,
	CMD_GetDeviceVersion = 0xC1,
	CMD_GetDeviceSerial = 0xC2
};

struct RGBLED
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	uint8_t Brightness;
}__attribute__((packed));

struct DeviceButton
{
	uint8_t Modifier;
	uint8_t Button;
}__attribute__((packed));

typedef struct DeviceProfile
{
	struct RGBLED profileLED;
	//Row1
	struct DeviceButton Button1;
	struct DeviceButton Button2;
	struct DeviceButton Button3;
	//Row2
	struct DeviceButton Button4;
	struct DeviceButton Button5;
	struct DeviceButton Button6;
	//Row3
	struct DeviceButton Button7;
	struct DeviceButton Button8;
	struct DeviceButton Button9;
}__attribute__((packed)) DeviceProfile;

enum KeyboardModifer
{
	MODIFIER_LEFTCTRL = 1,
	MODIFIER_LEFTSHIFT = 2,
	MODIFIER_LEFTALT = 4,
	MODIFIER_LEFTGUI = 8,
	MODIFIER_RIGHTCTRL = 16,
	MODIFIER_RIGHTSHIFT = 32,
	MODIFIER_RIGHTALT = 64,
	MODIFIER_RIGHTGUI = 128
};

typedef struct MacroPad_KeyboardReport
{
	uint8_t modifier;
	uint8_t reservedByte;
	uint8_t buttons[9];
}__attribute__((packed)) MacroPad_KeyboardReport;

struct KBStateStructure
{
	uint16_t B1 : 1;
	uint16_t B2 : 1;
	uint16_t B3 : 1;
	uint16_t B4 : 1;
	uint16_t B5 : 1;
	uint16_t B6 : 1;
	uint16_t B7 : 1;
	uint16_t B8 : 1;
	uint16_t B9 : 1;
	uint16_t CLEANUP : 1;
	uint16_t : 6;
}__attribute__((packed));

union KBStateTrack
{
	uint16_t STATE;
	struct KBStateStructure BITS;
};

#define COMMANDBUFFERSIZE 10
struct CommandBufferStruct
{
	uint8_t buffer[COMMANDBUFFERSIZE];
	uint8_t wP;
	uint8_t rP;
};

void SleepDevice(void);
void WakeDevice(void);
void Debounce_Handler(void);
uint8_t ProcessInputs(MacroPad_KeyboardReport*);
void CommandParse(const uint8_t*, const uint8_t);
uint8_t CommandBufferProcess(void);
void CopyDeviceVersionToBuffer(uint8_t*);
void CopyDeviceSerialNumberToBuffer(uint8_t*);
void CopyProfileToBuffer(uint8_t*);
void SaveProfile(void);
void LoadProfile(void);

#endif /* MACROPAD_H_ */