/*
 * macropad.h
 *
 * Created: 9/24/2020 3:10:10 PM
 *  Author: zlitt
 */ 


#ifndef MACROPAD_H_
#define MACROPAD_H_

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

void fillTestProfile(void);
uint8_t ProcessInputs(MacroPad_KeyboardReport*);

#endif /* MACROPAD_H_ */