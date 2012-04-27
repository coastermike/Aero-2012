#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"
#include "wheels.h"

unsigned char TLState = 0, BLState = 0, TRState = 0, BRState = 0; //States of individual hall effect sensors.
unsigned char LState = 1, RState = 1;			//State of present pin
unsigned int takeoff = 0;
unsigned int landing = 0;
unsigned int leftWheelTakeoff = 0;
unsigned int rightWheelTakeoff = 0;
unsigned int leftWheelLanding = 0;
unsigned int rightWheelLanding = 0;
unsigned int tempRightCount = 0, tempLeftCount = 0;
unsigned int wowL = 0;
unsigned int wowR = 0;
unsigned int wowCal = 0;
unsigned int IR = 0;
unsigned int mode = 0;

unsigned int tempcount = 0, tempcount1 = 0;
unsigned int tempClear = 0;

unsigned int calGroundL = 0, calGroundR = 0, calAirL = 0, calAirR = 0, calWowL = 0, calWowR = 0;
unsigned int calGndComplete = 0, calAirComplete = 0;
extern unsigned int stateOf;

void __attribute__((__interrupt__, no_auto_psv)) _CompInterrupt(void)
{
	if(_C1EVT)//WOW R
	{
		
	}
	if(_C2EVT)//IR
	{
//		LED3 = 1;
	}
	if(_C3EVT)//WOW L
	{
		
	}
	_CMIF = 0;
	CM2CONbits.CEVT = 0;
	CM3CONbits.CEVT = 0;
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
			tempcount = 3;
		}
		else
		{
			tempcount = 2;
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
				switch(LState)
				{
					case 1:
						if(tempcount == 3)
						{
							LState = 3;
							if(stateOf == 0)
							{
								leftWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempLeftCount++;
							}
							else if(stateOf == 6)
							{
								leftWheelLanding++;
							}	
						}
						break;
					case 2:
						if(tempcount == 1)
						{
							LState = 1;
							if(stateOf == 0)
							{
								leftWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempLeftCount++;
							}
							else if(stateOf == 6)
							{
								leftWheelLanding++;
							}
						}
						break;
					case 3:
						if(tempcount == 2)
						{
							LState = 2;
							if(stateOf == 0)
							{
								leftWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempLeftCount++;
							}
							else if(stateOf == 6)
							{
								leftWheelLanding++;
							}
						}
						break;
				}			
			}	
		}
		INTCON2bits.INT1EP = 1;
	}
//	mode = LState;
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
			tempcount = 3;
		}
		else
		{
			tempcount = 1;
		}
		INTCON2bits.INT2EP = 0;
	}
	else
	{
		if(!BLState)
		{
			if(!TLState)
			{
				switch(LState)
				{
					case 1:
						if(tempcount == 3)
						{
							LState = 3;
							if(stateOf == 0)
							{
								leftWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempLeftCount++;
							}
							else if(stateOf == 6)
							{
								leftWheelLanding++;
							}
						}
						break;
					case 2:
						if(tempcount == 1)
						{
							LState = 1;
							if(stateOf == 0)
							{
								leftWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempLeftCount++;
							}
							else if(stateOf == 6)
							{
								leftWheelLanding++;
							}
						}
						break;
					case 3:
						if(tempcount == 2)
						{
							LState = 2;
							if(stateOf == 0)
							{
								leftWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempLeftCount++;
							}
							else if(stateOf == 6)
							{
								leftWheelLanding++;
							}
						}
						break;
				}
			}	
		}
		INTCON2bits.INT2EP = 1;	
		
	}
//	mode = LState;

}

//Right Top sensor
void __attribute__((interrupt, no_auto_psv)) _INT3Interrupt (void)
{
	_INT3IF = 0;
	Nop();
	TRState = rightHallTop;
	
	if(TRState)
	{
		if(BRState)
		{
			tempcount1 = 3;
		}
		else
		{
			tempcount1 = 2;
		}
		INTCON2bits.INT3EP = 1;
	}
	else
	{
		//detect going from pins to no pins
		if(!TRState)
		{
			if(!BRState)
			{
				switch(RState)
				{
					case 1:
						if(tempcount1 == 2)
						{
							RState = 2;
							if(stateOf == 0)
							{
								rightWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempRightCount++;
							}
							else if(stateOf == 6)
							{
								rightWheelLanding++;
							}
						}
						break;
					case 2:
						if(tempcount1 == 3)
						{
							RState = 3;
							if(stateOf == 0)
							{
								rightWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempRightCount++;
							}
							else if(stateOf == 6)
							{
								rightWheelLanding++;
							}
						}
						break;
					case 3:
						if(tempcount1 == 1)
						{
							RState = 1;
							if(stateOf == 0)
							{
								rightWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempRightCount++;
							}
							else if(stateOf == 6)
							{
								rightWheelLanding++;
							}
						}
						break;
				}			
			}	
		}
		INTCON2bits.INT3EP = 0;
	}
//	mode = RState;
//	landing = rightWheelTakeoff;
}

//Right Bottom sensor
void __attribute__((interrupt, no_auto_psv)) _INT4Interrupt (void)
{
	_INT4IF = 0;
	Nop();
	BRState = rightHallBottom;
	
	if(BRState)
	{
		if(TRState)
		{
			tempcount1 = 3;
		}
		else
		{
			tempcount1 = 1;
		}
		INTCON2bits.INT4EP = 1;
	}
	else
	{
		if(!BRState)
		{
			if(!TRState)
			{
				switch(RState)
				{
					case 1:
						if(tempcount1 == 2)
						{
							RState = 2;
							if(stateOf == 0)
							{
								rightWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempRightCount++;
							}
							else if(stateOf == 6)
							{
								rightWheelLanding++;
							}
						}
						break;
					case 2:
						if(tempcount1 == 3)
						{
							RState = 3;
							if(stateOf == 0)
							{
								rightWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempRightCount++;
							}
							else if(stateOf == 6)
							{
								rightWheelLanding++;
							}
						}
						break;
					case 3:
						if(tempcount1 == 1)
						{
							RState = 1;
							if(stateOf == 0)
							{
								rightWheelTakeoff++;
							}
							else if((stateOf == 2) || (stateOf == 5))
							{
								tempRightCount++;
							}
							else if(stateOf == 6)
							{
								rightWheelLanding++;
							}
						}
						break;
				}
			}	
		}
		INTCON2bits.INT4EP = 0;	
	}
//	mode = RState;
//	landing = rightWheelTakeoff;
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
	INTCON2bits.INT3EP = 0;
	INTCON2bits.INT4EP = 0;
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
//	CM1CONbits.COE = 0;			//disable output pin
//	CM1CONbits.CPOL = 1;		//invert sense
//	CM1CONbits.EVPOL = 0b00;	//no event detection
//	CM1CONbits.CREF = 1;		//CVref
//	CM1CONbits.CCH = 0;			//INB
//	CM1CONbits.CON = 1;
	
	//IR V on A and IR sensor on B
	CM2CONbits.COE = 0;			//disable output pin
	CM2CONbits.CPOL = 1;		//invert sense
	CM2CONbits.EVPOL = 0b10;	//no event detection
	CM2CONbits.CREF = 0;		//INA
	CM2CONbits.CCH = 0;			//INB
	CM2CONbits.CON = 1;
	
	//WOW L on D
//	CM3CONbits.COE = 0;		//disable output pin
//	CM3CONbits.CPOL = 1;	//invert sense
//	CM3CONbits.EVPOL = 0b00;	//no event detection
//	CM3CONbits.CREF = 1;		//CVref
//	CM3CONbits.CCH = 0b10;		//IND
//	CM3CONbits.CON = 1;
	
	CVRCONbits.CVREN = 1;
	CVRCONbits.CVROE = 0;
	CVRCONbits.CVRR = 1;	//scale
	CVRCONbits.CVRSS = 0;
	CVRCONbits.CVR = 8;		//vref setting
	
	_CMIF = 0;
//	CM1CONbits.CEVT = 0;
	CM2CONbits.CEVT = 0;
//	CM3CONbits.CEVT = 0;
	_CMIE = 1;
}

void calibrateWOW()
{
	
}

void calGround()
{
	AD1CON1bits.ASAM = 1;
	while(AD1CON1bits.ASAM && _AD1IF);
	calGroundL = wowL;
	calGroundR = wowR;

	calGndComplete = 1;
}

void calAir()
{
	AD1CON1bits.ASAM = 1;
	while(AD1CON1bits.ASAM && _AD1IF);
	calAirL = wowL;
	calAirR = wowR;
	
	if(calGndComplete == 1)
	{
		calWowL = (unsigned int)(calAirL - calGroundL) * 3 / 4 + calGroundL;
		calWowR = (unsigned int)(calAirR - calGroundR) * 3 / 4 + calGroundR;
		wowCal=calWowL;
		calAirComplete = 1;
	}	
}

void reset()
{
	takeoff = 0;
	landing = 0;
	leftWheelTakeoff = 0;
	rightWheelTakeoff = 0;
	leftWheelLanding = 0;
	rightWheelLanding = 0;
	
	stateOf = 0;
	calGndComplete = 0;
	calAirComplete = 0;
}

void start()
{
	if(stateOf == 43)
	{
		stateOf = 0;
	}	
}
