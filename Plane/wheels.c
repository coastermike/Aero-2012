#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"
#include "wheels.h"

unsigned char TLState = 0, BLState = 0, TRState = 0, BRState = 0; //States of individual hall effect sensors.
unsigned char LState = 0, RState = 0;			//State of present pin
unsigned int takeoff = 0;
unsigned int landing = 0;
unsigned int leftWheelTakeoff = 0;
unsigned int rightWheelTakeoff = 0;
unsigned int leftWheelLanding = 0;
unsigned int rightWheelLanding = 0;
unsigned int wowL = 0;
unsigned int wowR = 0;
unsigned int wowCal = 0;
unsigned int IR = 0;
unsigned int mode = 0;

unsigned int tempcount = 0;
unsigned int tempClear = 0;

void __attribute__((__interrupt__, no_auto_psv)) _CompInterrupt(void)
{
	if(CMSTAT & _C1EVT)//WOW R
	{
		LED2 = 1;
		LED1 = 0;
	}
	if(_C2EVT)//IR
	{
		LED3 = 1;
	}
	if(CMSTAT & _C3EVT)//WOW L
	{
		LED1 = 1;
		LED2 = 0;
	}
	_CMIF = 0;
	CM2CONbits.CEVT = 0;
}

//Left Top sensor
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt (void)
{
	_INT1IF = 0;
	
	TLState = !leftHallTop;
	
	if(TLState)
	{
		if(BLState)
		{
			if(LState == 0)
			{
				tempcount = 1;
				leftWheelTakeoff++;
				LState = 5;
			}
		}
		else if(LState == 4)
		{
			//increase count
			tempcount = 1;
			leftWheelTakeoff++;
			LState = 3;
		}
		INTCON2bits.INT1EP = 0;
	}
	else
	{
		//detect going from pins to no pins
		if(!TLState)
		{
			if(!BLState)
			{
				if(((LState%2) != 0) && (tempcount == 1))
				{
					LState--;
					tempcount = 0;
				}	
			}	
		}
		INTCON2bits.INT1EP = 1;
	}
	mode = LState;
	landing = leftWheelTakeoff;
}

//Left Bottom sensor
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt (void)
{
	_INT2IF = 0;
	
	BLState = !leftHallBottom;
	
	if(BLState)
	{
		if(TLState)
		{
			if(LState == 0)
			{
				//increase count
				tempcount = 1;
				leftWheelTakeoff++;
				LState = 5;
			}
//			INTCON2bits.INT1EP = 1;
		}
		else if(LState == 2)
		{
			//increase count
			tempcount = 1;
			leftWheelTakeoff++;
			LState = 1;
		}
		INTCON2bits.INT2EP = 0;
	}
	else
	{
		if(!BLState)
		{
			if(!TLState)
			{
				if(((LState%2) != 0) && (tempcount == 1))
				{
					LState--;
					tempcount = 0;
				}
			}	
		}
		INTCON2bits.INT2EP = 1;	
		
	}
	mode = LState;
	landing = leftWheelTakeoff;
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
	TRISDbits.TRISD8 = 1;
	TRISDbits.TRISD9 = 1;
	TRISDbits.TRISD15 = 1;
	TRISFbits.TRISF4 = 1;
	//INT1-4 for sensors
	RPINR0bits.INT1R = 4;	//HallTop_L INT1 on RP4
	RPINR1bits.INT2R = 2;	//HallBottom_L INT2 on RP2
	RPINR1bits.INT3R = 10;	//HallTop_R INT3 on RP10
	RPINR2bits.INT4R = 5;	//HallBottom_R INT4 on RP5
	INTCON2bits.INT1EP = 1;	//INT on negative edge
	INTCON2bits.INT2EP = 1;
	INTCON2bits.INT3EP = 1;
	INTCON2bits.INT4EP = 1;
	_INT1IE = 1;
	_INT2IE = 1;
	_INT3IE = 1;
	_INT4IE = 1;
	_INT1IP = 5;		//interrupt priority at 5
	_INT2IP = 5;
	_INT3IP = 5;
	_INT4IP = 5;
	
	//init for ir, pressure sensors comparators
	TRISBbits.TRISB4 = 1;
	TRISBbits.TRISB3 = 1;
	TRISBbits.TRISB2 = 1;
	TRISCbits.TRISC13 = 1;
	AD1PCFGLbits.PCFG2 = 0;
	AD1PCFGLbits.PCFG3 = 0;
	AD1PCFGLbits.PCFG4 = 0;
	
	//WOW R on B
	CM1CONbits.COE = 0;			//disable output pin
	CM1CONbits.CPOL = 1;		//invert sense
	CM1CONbits.EVPOL = 0b00;	//no event detection
	CM1CONbits.CREF = 1;		//CVref
	CM1CONbits.CCH = 0;			//INB
	CM1CONbits.CON = 1;
	
	//IR V on A and IR sensor on B
	CM2CONbits.COE = 0;			//disable output pin
	CM2CONbits.CPOL = 1;		//invert sense
	CM2CONbits.EVPOL = 0b10;	//no event detection
	CM2CONbits.CREF = 0;		//INA
	CM2CONbits.CCH = 0;			//INB
	CM2CONbits.CON = 1;
	
	//WOW L on D
	CM3CONbits.COE = 0;		//disable output pin
	CM3CONbits.CPOL = 1;	//invert sense
	CM3CONbits.EVPOL = 0b00;	//no event detection
	CM3CONbits.CREF = 1;		//CVref
	CM3CONbits.CCH = 0b10;		//IND
	CM3CONbits.CON = 1;
	
	CVRCONbits.CVREN = 1;	//turn module on
	CVRCONbits.CVROE = 0;	//disable output pin
	CVRCONbits.CVRR = 0;	//higher range
	CVRCONbits.CVRSS = 0;
	CVRCONbits.CVR = 0;		//sets voltage reference
	
	_CMIF = 0;
	CM1CONbits.CEVT = 0;
	CM2CONbits.CEVT = 0;
	CM3CONbits.CEVT = 0;
	_CMIE = 1;
}

void calibrateWOW()
{
	
}
