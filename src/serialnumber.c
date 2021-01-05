/*
 * serialnumber.c
 *
 * Created: 11/11/2020 9:38:46 AM
 *  Author: zlitt
 */ 

#include "serialnumber.h"

const char __attribute__ ((section (".serialNumberSection"))) DeviceSerialNumber[DEVICESERIALNUMBERLENGTH] = 
{
	"0123456789"
};