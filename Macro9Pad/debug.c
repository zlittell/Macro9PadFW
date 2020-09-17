/*
 * debug.c
 *
 * Created: 9/17/2020 1:19:22 PM
 *  Author: zlitt
 */ 

#include <sam.h>

void debugEnableClockOutputs(void)
{
	//Make sure to add OutputEnable to GCLK setup
	//Set up clock outputs
	//clock 0 8MHz
	PORT->Group[0].DIRSET.reg = PORT_PA08;
	PORT->Group[0].PINCFG[8].reg |= PORT_PINCFG_PMUXEN;
	//PORT->Group[0].PMUX[4].reg = PORT_PMUX_PMUXE_H;
	//clock1 48MHz
	PORT->Group[0].DIRSET.reg = PORT_PA09;
	PORT->Group[0].PINCFG[9].reg = PORT_PINCFG_PMUXEN;
	PORT->Group[0].PMUX[4].reg = (PORT_PMUX_PMUXE_H | PORT_PMUX_PMUXO_H);
	//clock 3 should be 32khz
	PORT->Group[0].DIRSET.reg = PORT_PA17;
	PORT->Group[0].PINCFG[17].reg |= PORT_PINCFG_PMUXEN;
	PORT->Group[0].PMUX[8].reg = PORT_PMUX_PMUXO_H;
}