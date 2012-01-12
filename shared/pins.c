#include "pins.h"
#include <p24FJ256GA110.h>

//Sets the Tris bits of the pins that are not part of a peripheral
void Pin_Init()
{
	//setting all Tris bits
	LED1Tris = 0;
	LED2Tris = 0;
	LED3Tris = 0;
	LED4Tris = 0;
	LED5Tris = 0;
	LEDStatusTris = 0;
}
