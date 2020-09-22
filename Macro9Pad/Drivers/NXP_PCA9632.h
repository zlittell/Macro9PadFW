/*
 * NXP_PCA9632.h
 *
 * Created: 9/21/2020 9:17:08 PM
 *  Author: zlitt
 */ 


#ifndef NXP_PCA9632_H_
#define NXP_PCA9632_H_

#define LEDADDR 0x60
#define PCARESETADDR 0x03

void LED_init(void);
void LED_updateRed(uint8_t);
void LED_updateGreen(uint8_t);
void LED_updateBlue(uint8_t);
void LED_updateRGB(uint8_t, uint8_t, uint8_t);
void LED_updateBrightness(void);

#endif /* NXP_PCA9632_H_ */