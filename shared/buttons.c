#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"
#include "../shared/buttons.h"

unsigned char sw1State = 0, sw2State = 0, sw3State = 0;
unsigned char sw1Temp = 0, sw2Temp = 0, sw3Temp = 0;

//Note: buttonX=1 represents off since pull up
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
	_CNIF = 0;
	if(!button1 && !sw1State && (sw1Temp == 0))
	{
		sw1Temp = 1;
	}
	else if(button1 && !sw1State && (sw1Temp == 1))
	{
		sw1Temp = 2;
	}
	if(!button2 && !sw2State && (sw2Temp == 0))
	{
		sw2Temp = 1;
	}
	else if(button2 && !sw2State && (sw2Temp == 1))
	{
		sw2Temp = 2;
	}
	if(!button3 && !sw1State && (sw3Temp == 0))
	{
		sw3Temp = 1;
	}
	else if(button3 && !sw3State && (sw3Temp == 1))
	{
		sw3Temp = 2;
	}			
}

void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
	if((sw1Temp == 2) && button1)
	{
		sw1Temp++;
	}
	else if((sw1Temp == 3) && button1)
	{
		sw1Temp = 0;
		sw1State = 1;
	}
	if((sw2Temp == 2) && button2)
	{
		sw2Temp++;
	}
	else if((sw2Temp == 3) && button2)
	{
		sw2Temp = 0;
		sw2State = 1;
	}
	if((sw3Temp == 2) && button3)
	{
		sw3Temp++;
	}
	else if((sw3Temp == 3) && button3)
	{
		sw3Temp = 0;
		sw3State = 1;
	}
	_T3IF = 0;
}

void initButtons()
{
	CNEN5bits.CN79IE = 1;	//enables 79, 80, 81 as change notification
	CNEN6bits.CN80IE = 1;
	CNEN6bits.CN81IE = 1;
	CNPU5bits.CN79PUE = 1;	//enables the weak pull up resistor for 79, 80, 81
	CNPU6bits.CN80PUE = 1;
	CNPU6bits.CN81PUE = 1;
	
	T3CONbits.TCKPS = 0b10;
	T3CONbits.TCS = 0;
	T3CONbits.TGATE = 0;
	PR3 = 2500;
	T3CONbits.TON = 1;
	_T3IF = 0;
	_T3IP = 3;
	_T3IE = 1;
	
	_CNIF = 0;				//clears interrupt flag
	_CNIP = 4;				//sets interrupt priority
	_CNIE = 1;				//enables interrupt
}

unsigned int swState(unsigned int sw)
{
	if(sw == 1)
	{
		return sw1State;
	}
	else if(sw == 2)
	{
		return sw2State;
	}
	else if(sw == 3)
	{
		return sw3State;
	}
	else
	{
		return 0;
	}				
}

void clearSW()
{
	sw1State = 0;
	sw2State = 0;
	sw3State = 0;
}
