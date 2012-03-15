#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"

unsigned int brakeL = 0;
unsigned int brakeR = 0;

unsigned long in1 = 0;
unsigned long in2 = 0;

void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void)
{
	IC1CON2bits.TRIGSTAT = 0;
	in1 = IC2BUF;
	in1 = (in1 << 8) | IC1BUF;
	brakeL = in1;
	IC1CON2bits.TRIGSTAT = 1;
	_IC1IF = 0;
}	

void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void)
{
	IC3CON2bits.TRIGSTAT = 0;
	in2 = IC4BUF;
	in2 = (in2 << 8) | IC3BUF;
	brakeR = in2;
	IC3CON2bits.TRIGSTAT = 1;
	_IC3IF = 0;
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
	IC2CON2bits.IC32 = 1;
	IC1CON2bits.IC32 = 1;
	IC2CON1bits.ICTSEL = 0b111;
	IC2CON2bits.SYNCSEL = 0;
	IC1CON1bits.ICTSEL = 0b111;
	IC1CON2bits.SYNCSEL = 0;
	IC2CON2bits.ICTRIG = 1;
	IC1CON2bits.ICTRIG = 1;
	IC1CON1bits.ICM = 0b001;
	//BrakeR input IC2
	IC4CON2bits.IC32 = 1;
	IC3CON2bits.IC32 = 1;
	IC4CON1bits.ICTSEL = 0b111;
	IC4CON2bits.SYNCSEL = 0;
	IC3CON1bits.ICTSEL = 0b111;
	IC3CON2bits.SYNCSEL = 0;
	IC2CON2bits.ICTRIG = 1;
	IC3CON2bits.ICTRIG = 1;
	IC3CON1bits.ICM = 0b001;
	_IC1IE = 1;
	_IC3IE = 1;
	_IC1IP = 4;
	_IC3IP = 4;
}
