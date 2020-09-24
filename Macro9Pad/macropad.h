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
};

#endif /* MACROPAD_H_ */