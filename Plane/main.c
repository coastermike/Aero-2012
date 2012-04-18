#include <p24FJ256GA110.h>
#include "../shared/status.h"
#include "../shared/LEDs.h"
#include "../shared/uart.h"
#include "../shared/buttons.h"
#include "wheels.h"
#include "brakes.h"
#include "accel.h"
#include "analog.h"

//Config Bits set here
//Reference p24FJ256GA110.h
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & COE_OFF  & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & IOL1WAY_OFF & FNOSC_FRCPLL)

int main(void)
{	
	CLKDIVbits.RCDIV = 0b000;
	CLKDIVbits.DOZE = 0b000;
	INTCON1bits.NSTDIS = 1; //Disables interrupt nesting
	initLEDs();
	initAnalog();
	initStatus();
	initUart();
	initButtons();
	initWheels();
	initBrakes();
	initAccel();
	OC1R = 0;
	OC2R = 0;
	while(1)
	{	
		AD1CON1bits.ASAM = 1;
		if(CMSTATbits.C1OUT)//IR
		{
//			LED3 = 1;
		}
		else
		{
//			LED3 = 0;
		}
		
		//test code to make LEDs blink to hall effects
		if(rightHallTop)
		{
			LED1 = 1;
		}
		else
		{
			LED1 = 0;
		}
		if(rightHallBottom)
		{
			LED2 = 1;
		}
		else
		{
			LED2 = 0;
		}
		//end hall effect test code		
	}		
}	
