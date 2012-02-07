#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"

unsigned char TLState = 0, BLState = 0, TRState = 0, BRState = 0; //States of individual hall effect sensors.
unsigned char LState = 0, RState = 0;			//State of present pin

//Left Top sensor
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt (void)
{
	_INT1IF = 0;
	
	if(!TLState)
	{
		if(BLState)
		{
			if(LState == 1)
			{
				//increase count
			}	
			LState = 3;
			INTCON2bits.INT2EP = 0;
			BLState = 0;
		}
		else
		{
			INTCON2bits.INT1EP = 1;
			TLState = 1;
		}
	}
	else
	{
		if(LState == 3)
		{
			//increase count
		}	
		LState = 2;
		INTCON2bits.INT1EP = 0;
		TLState = 0;
	}
}

//Left Bottom sensor
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt (void)
{
	_INT2IF = 0;
	
	if(!BLState)
	{
		if(TLState)
		{
			if(LState == 1)
			{
				//increase count
			}	
			LState = 3;
			INTCON2bits.INT1EP = 0;
			TLState = 0;
		}
		else
		{
			INTCON2bits.INT2EP = 1;
			BLState = 1;
		}
	}
	else
	{
		if(LState == 2)
		{
			//increase count
		}	
		LState = 1;
		INTCON2bits.INT2EP = 0;
		BLState = 0;
	}
}

//Right Top sensor
void __attribute__((interrupt, no_auto_psv)) _INT3Interrupt (void)
{
	_INT3IF = 0;
}

//Right Bottom sensor
void __attribute__((interrupt, no_auto_psv)) _INT4Interrupt (void)
{
	_INT4IF = 0;
}

void initWheels()
{
	//INT1-4 for sensors
	RPINR0bits.INT1R = 4;	//HallTop_L INT1 on RP4
	RPINR1bits.INT2R = 2;	//HallBottom_L INT2 on RP2
	RPINR1bits.INT3R = 10;	//HallTop_R INT3 on RP10
	RPINR2bits.INT4R = 5;	//HallBottom_R INT4 on RP5
	INTCON2bits.INT1EP = 0;	//INT on positive edge
	INTCON2bits.INT2EP = 0;
	INTCON2bits.INT3EP = 0;
	INTCON2bits.INT4EP = 0;
	_INT1IE = 1;
	_INT2IE = 1;
	_INT3IE = 1;
	_INT4IE = 1;
	_INT1IP = 0b101;		//interrupt priority at 5
	_INT2IP = 0b101;
	_INT3IP = 0b101;
	_INT4IP = 0b101;
	
	//init for ir, pressure sensors
	
}
