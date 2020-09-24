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
};

struct DeviceButton
{
	uint8_t Modifier;
	uint8_t Button;
};

struct DeviceProfile
{
	RGBLED profileLED;
	//Row1
	DeviceButton Button1;
	DeviceButton Button2;
	DeviceButton Button3;
	//Row2
	DeviceButton Button4;
	DeviceButton Button5;
	DeviceButton Button6;
	//Row3
	DeviceButton Button7;
	DeviceButton Button8;
	DeviceButton Button9;
};

#endif /* MACROPAD_H_ */