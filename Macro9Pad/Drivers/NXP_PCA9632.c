/*
 * NXP_PCA9632.c
 *
 * Created: 9/21/2020 9:16:48 PM
 *  Author: zlitt
 */ 

#include <stdint.h>
#include "NXP_PCA9632.h"
#include "MSF_I2C.h"

#define PCA_ResetCMD_Length 2
const uint8_t PCA_ResetCMD[PCA_ResetCMD_Length] = {0xA5, 0x5A};
const uint8_t PCA_AUTOINC_NONE = (0u << 5);
const uint8_t PCA_AUTOINC_ALL = (4u << 5);
const uint8_t PCA_AUTOINC_BRIGHTNESS = (5u << 5);
const uint8_t PCA_AUTOINC_GLOBAL = (6u << 5);
const uint8_t PCA_AUTOINC_INDIVIDUALANDGLOBAL = (7u << 5);
const uint8_t PCA_REG_MODE1 = (0u);
const uint8_t PCA_REG_MODE2 = (1u);
const uint8_t PCA_REG_PWM0 = (2u);
const uint8_t PCA_REG_PWM1 = (3u);
const uint8_t PCA_REG_PWM2 = (4u);
const uint8_t PCA_REG_PWM3 = (5u);
const uint8_t PCA_REG_GRPPWM = (6u);
const uint8_t PCA_REG_GRPFREQ = (7u);
const uint8_t PCA_REG_LEDOUT = (8u);
const uint8_t PCA_REG_SUBADR1 = (9u);
const uint8_t PCA_REG_SUBADR2 = (10u);
const uint8_t PCA_REG_SUBADR3 = (11u);
const uint8_t PCA_REG_ALLCALLADR = (12u);
const uint8_t PCA_MODE1_SLEEP = (1u << 4);
const uint8_t PCA_MODE1_SUB1 = (1u << 3);
const uint8_t PCA_MODE1_SUB2 = (1u << 2);
const uint8_t PCA_MODE1_SUB3 = (1u << 1);
const uint8_t PCA_MODE1_ALLCALL = (1u << 0);
const uint8_t PCA_MODE2_DMBLNK = (1u << 5);
const uint8_t PCA_MODE2_INVRT = (1u << 4);
const uint8_t PCA_MODE_OCH = (1u << 3);
const uint8_t PCA_MODE_OUTDRV = (1u << 2);
const uint8_t RGBLED_Red = (4u);
const uint8_t RGBLED_Green = (3u);
const uint8_t RGBLED_Blue = (2u);

void LED_init(void)
{
	//reset device
	i2c_send(PCARESETADDR, (uint8_t*)PCA_ResetCMD, PCA_ResetCMD_Length);	
}

void LED_updateRed(uint8_t value)
{
	uint8_t message[2] = {(RGBLED_Red | PCA_AUTOINC_NONE), value};
	i2c_send(LEDADDR, message, 2);
}

void LED_updateGreen(uint8_t value)
{
	uint8_t message[2] = {(RGBLED_Green | PCA_AUTOINC_NONE), value};
	i2c_send(LEDADDR, message, 2);
}

void LED_updateBlue(uint8_t value)
{
	uint8_t message[2] = {(RGBLED_Blue | PCA_AUTOINC_NONE), value};
	i2c_send(LEDADDR, message, 2);
}

void LED_updateRGB(uint8_t value1, uint8_t value2, uint8_t value3)
{
	//Goes Blue, Green, Red
	uint8_t message[4] = {(PCA_REG_PWM0 | PCA_AUTOINC_ALL), value1, value2, value3};
	i2c_send(LEDADDR, message, 4);
}

void LED_updateBrightness(void)
{
	
}