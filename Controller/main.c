#include <p24FJ256GA110.h>
#include "../shared/status.h"
#include "../shared/LEDs.h"
#include "../shared/uart.h"
#include "../shared/buttons.h"
#include "gLCD.h"

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
	initLCD();
//	lcd_screenon(1);
//	Nop();
//	Nop();
//	lcd_plotpixel(5,5);

	RW=0;// DI=0;
	PMADDR = 0x0000;
	PMDIN1 = 0b00111111;
	msDelay(100);
	CS1=1;
	CS2=0;
	PMADDR = 0;
	PMDIN1 = 0b10111000 | 0b001;
	msDelay(100);
	PMADDR=0;
	PMDIN1 = 0b01000000 | (0xF & 0b00111111);
	msDelay(100);
	PMADDR=1;
	PMDIN1 = 0x0;
	
	while(1)
	{
//		LED1 = !sw1LastState;
//		LED2 = !sw2LastState;
//		LED3 = !sw3LastState;
//		writeUart(1);
	}	
}
