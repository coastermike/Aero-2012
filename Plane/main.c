#include <p24FJ256GA110.h>
#include "../shared/status.h"
#include "../shared/LEDs.h"
#include "../shared/uart.h"
#include "../shared/buttons.h"
#include "wheels.h"
#include "brakes.h"

//Config Bits set here
//Reference p24FJ256GA110.h
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & COE_OFF  & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & IOL1WAY_OFF & FNOSC_FRCPLL)

extern unsigned char sw1LastState;
extern unsigned char sw2LastState;
extern unsigned char sw3LastState;

int main(void)
{	
	CLKDIVbits.RCDIV = 0b000;
	CLKDIVbits.DOZE = 0b000;
	INTCON1bits.NSTDIS = 1; //Disables interrupt nesting
	initLEDs();
	initStatus();
	initUart();
	initButtons();
	initWheels();
	initBrakes();
	while(1)
	{
		LED1 = !sw1LastState;
		LED2 = !sw2LastState;
		LED3 = !sw3LastState;
		if(!sw1LastState)
		{
			writeUart(1);
		}	
		if(!sw2LastState)
		{
			writeUart(2);
		}
		if(!sw3LastState)
		{
			writeUart(3);
		}
		if(sw1LastState && sw2LastState && sw3LastState)
		{
			writeUart(0);
		}
		
		//test code to make LEDs blink to hall effects
		if(leftHallTop)
		{
			LED4 = 1;
		}
		else
		{
			LED4 = 0;
		}
		if(leftHallBottom)
		{
			LED5 = 1;
		}
		else
		{
			LED5 = 0;
		}
		//end hall effect test code		
	}		
}	
