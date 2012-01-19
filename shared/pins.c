#include <p24FJ256GA110.h>
#include "../shared/pins.h"

//Sets the Tris bits of the pins that are not part of a peripheral
void initPins()
{
	AD1PCFGL = 0xFFFF;	//sets all pins to digital
	//setting all Tris bits
	LED1Tris = 0;
	LED2Tris = 0;
	LED3Tris = 0;
	LED4Tris = 0;
	LED5Tris = 0;
	LEDStatusTris = 0;
	
	//switch pins as inputs
	TRISGbits.TRISG12 = 1;
	TRISGbits.TRISG13 = 1;
	TRISGbits.TRISG14 = 1;
}
