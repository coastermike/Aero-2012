#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"
#include "brakes.h"

#define BRAKE_SCALE 510 //510 is max for 0% to 100% output, make smaller to limit max

unsigned long brakeSteer = 0;
unsigned long brakeMag = 0;

unsigned int in1 = 0;
unsigned int in2 = 0;
unsigned int left = 0, right = 0;

unsigned int tempOC1 = 0, tempOC2 = 0;
//steering input
//MIGHT need to change when left and right go to max settings - right now left hits max way sooner than the right side
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void)
{
	if(steer)
	{
		in1 = IC1BUF;
		IC1CON2bits.TRIGSTAT = 0;
		IC1CON2bits.TRIGSTAT = 1;
	}	
	else if(!steer)
	{
		in1 = IC1BUF;
		IC1CON2bits.TRIGSTAT = 0;
		IC1CON2bits.TRIGSTAT = 1;
		if((in1 > 33360) && (in1 < 50000)) //go full left
		{
			left = 100;
			right = 0;
			LED3 = 0;
		}
		else if((in1 < 27280) && (in1 > 0)) //go full right
		{
			right = 100;
			left = 0;
			LED3 = 0;
		}
		else if((in1 > 30387) && (in1 < 30487)) //go straight
		{
			right = 100;
			left = 100;
			LED3 = 0;
		}
		else
		{	
			brakeSteer = (20 * (unsigned long)in1 / 1217) - 448;
			LED3 = 1;
			if((brakeSteer > 0) && (brakeSteer < 50))
			{
				left = brakeSteer * 2;
				right = 100;
			}
			else if((brakeSteer > 50) && (brakeSteer < 100))
			{
				left = 100;
				right = 2 * (100 - brakeSteer);
			}
		}
		setBrakes();	
	}
	_IC1IF = 0;
}

//magnitude of brake level reading
void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void)
{
	if(magnitude)
	{
		in2 = IC3BUF;
		IC3CON2bits.TRIGSTAT = 0;
		IC3CON2bits.TRIGSTAT = 1;
	}	
	else if(!magnitude)
	{
		in2 = IC3BUF;
		IC3CON2bits.TRIGSTAT = 0;
		IC3CON2bits.TRIGSTAT = 1;
		if(in2 > 22000)
		{
			brakeMag = 0;
		}
		else if(in2 > 20850)
		{
			brakeMag = 100;
		}
		else if((in2 < 14700) && (in2 > 0))
		{
			brakeMag = 0;
		}
		else
		{
			brakeMag = 4 * (unsigned long)in2 / 249 - 235;
		}
		setBrakes();				
	}
	_IC3IF = 0;
}	

void setBrakes()
{
	tempOC1 = BRAKE_SCALE * brakeMag * left / 100;
	tempOC2 = BRAKE_SCALE * brakeMag * right / 100;
	OC1R = BRAKE_SCALE * brakeMag * left / 100;
	OC2R = BRAKE_SCALE * brakeMag * right / 100;
	
	
}
	
void initBrakes()
{
	//Input capture and output compare settings
	//reports whichever is the smaller of the counts
	TRISFbits.TRISF3 = 1;
	TRISFbits.TRISF2 = 1;
	TRISDbits.TRISD1 = 0;
	TRISDbits.TRISD2 = 0;
	
	RPINR7bits.IC1R = 16;
	RPINR8bits.IC3R = 30;
	RPOR11bits.RP23R = 18;
	RPOR12bits.RP24R = 19;
	
	//PWM1 Left setup
	OC1CON1 = 0;
	OC1CON2 = 0;
	OC1CON1bits.OCTSEL = 0b111;
	OC1R = 0;			//duty cycle of 50%
	OC1RS = 51000;			//period
	OC1CON2bits.SYNCSEL = 0x1F;
	OC1CON1bits.OCM = 0b110;	//PWM, no fault
	//PWM2 right setup
	OC2CON1 = 0;
	OC2CON2 = 0;
	OC2CON1bits.OCTSEL = 0b111;
	OC2R = 0;			//duty cycle of 50%
	OC2RS = 51000;			//period
	OC2CON2bits.SYNCSEL = 0x1F;
	OC2CON1bits.OCM = 0b110;	//PWM, no fault
	
	//BrakeL input IC1
//	IC2CON2bits.IC32 = 1;
	IC1CON2bits.IC32 = 0;
//	IC2CON1bits.ICTSEL = 0b111;
//	IC2CON2bits.SYNCSEL = 0;
	IC1CON1bits.ICTSEL = 0b111;
	IC1CON2bits.SYNCSEL = 0;
//	IC2CON2bits.ICTRIG = 0;
	IC1CON2bits.ICTRIG = 1;
	IC1CON1bits.ICM = 0b001;
	//BrakeR input IC2
//	IC4CON2bits.IC32 = 1;
	IC3CON2bits.IC32 = 1;
//	IC4CON1bits.ICTSEL = 0b111;
//	IC4CON2bits.SYNCSEL = 0;
	IC3CON1bits.ICTSEL = 0b111;
	IC3CON2bits.SYNCSEL = 0;
	IC3CON2bits.ICTRIG = 1;
	IC3CON1bits.ICM = 0b001;
	_IC1IE = 1;
	_IC3IE = 1;
	_IC1IP = 4;
	_IC3IP = 4;
}
