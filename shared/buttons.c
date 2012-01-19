#include <p24FJ256GA110.h>
#include "../shared/pins.h"

unsigned char sw1LastState = 1;	//1 represents off since pull up
unsigned char sw2LastState = 1;
unsigned char sw3LastState = 1;

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
	_CNIF = 0;
	if(button1 != sw1LastState)
	{
		sw1LastState = button1;
	}
	if(button2 != sw2LastState)
	{
		sw2LastState = button2;
	}
	if(button3 != sw3LastState)
	{
		sw3LastState = button3;
	}			
}
	
void initButtons()
{
	CNEN5bits.CN79IE = 1;	//enables 79, 80, 81 as change notification
	CNEN6bits.CN80IE = 1;
	CNEN6bits.CN81IE = 1;
	CNPU5bits.CN79PUE = 1;	//enables the weak pull up resistor for 79, 80, 81
	CNPU6bits.CN80PUE = 1;
	CNPU6bits.CN81PUE = 1;
	
	_CNIF = 0;				//clears interrupt flag
	_CNIP = 4;				//sets interrupt priority
	_CNIE = 1;				//enables interrupt
}
