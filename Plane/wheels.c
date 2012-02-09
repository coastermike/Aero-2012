#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"

unsigned char TLState = 0, BLState = 0, TRState = 0, BRState = 0; //States of individual hall effect sensors.
unsigned char LState = 0, RState = 0;			//State of present pin

void __attribute__((__interrupt__, no_auto_psv)) _CompInterrupt(void)
{
	if(CMSTAT & _C1EVT)
	{
		
	}
	if(CMSTAT & _C2EVT)
	{
		
	}
	if(CMSTAT & _C3EVT)
	{
		
	}
	_CMIF = 0;
	CM1CONbits.CEVT = 0;
}

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
				//increase count USE VOLATILE???
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
	CM2CONbits.EVPOL = 0b00;	//no event detection
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
}

void calibrateWOW()
{
	
}
