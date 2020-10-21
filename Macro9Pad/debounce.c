/*
 * debounce.c
 *
 * Created: 10/21/2020 3:13:36 PM
 *  Author: zlitt
 */

#include "sam.h"
#include "macropad.h"

#define SWITCHDEBOUNCE 5
extern DeviceInputs InputState;

/**
*	@brief Handle button debounce.
*	@details Call on timer interrupt to process inputs while preventing bounce.
*/
void Debounce_Handler(void)
{
	static struct DeviceInput_Debounce InputDebounceCount = {0};
	
	//Button1 - PA23
	if(PORT->Group[0].IN.reg & PORT_PA23)
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
	
	//Button2 - PA02
	if(PORT->Group[0].IN.reg & PORT_PA02)
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
	
	//Button3 - PA03
	if(PORT->Group[0].IN.reg & PORT_PA03)
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
	
	//Button4 - PA11
	if(PORT->Group[0].IN.reg & PORT_PA11)
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
	
	//Button5 - PA06
	if(PORT->Group[0].IN.reg & PORT_PA06)
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
	
	//Button6 - PA04
	if(PORT->Group[0].IN.reg & PORT_PA04)
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
	
	//Button7 - PA10
	if(PORT->Group[0].IN.reg & PORT_PA10)
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
	
	//Button8 - PA07
	if(PORT->Group[0].IN.reg & PORT_PA07)
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
	
	//Button9 - PA05
	if(PORT->Group[0].IN.reg & PORT_PA05)
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
	
	//TestIO1 - PA17
	if(PORT->Group[0].IN.reg & PORT_PA17)
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
	
	//TestIO2 - PA16
	if(PORT->Group[0].IN.reg & PORT_PA16)
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