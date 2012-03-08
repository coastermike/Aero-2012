#include <p24FJ256GA110.h>
#include "../shared/status.h"
#include "../shared/LEDs.h"
#include "../shared/uart.h"
#include "../shared/buttons.h"
#include "gLCD.h"
#include "screens.h"

//Config Bits set here
//Reference p24FJ256GA110.h
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & COE_OFF  & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & IOL1WAY_OFF & FNOSC_FRCPLL)

#define CONTROLLER

extern unsigned char sw1LastState;
extern unsigned char sw2LastState;
extern unsigned char sw3LastState;

char st[] = "Hello";
int main(void)
{
	CLKDIVbits.RCDIV = 0b000;
	CLKDIVbits.DOZE = 0b000;
	INTCON1bits.NSTDIS = 1; //Disables interrupt nesting
	initLEDs();
	initStatus();
	initUart();
	initButtons();
	initLCD();
	lcd_screenon(1);
	lcd_cls();
	
	initScreens();
	
	while(1)
	{
		LED1 = !sw1LastState;
		LED2 = !sw2LastState;
		LED3 = !sw3LastState;
//		writeUart(1);	
	}	
}
