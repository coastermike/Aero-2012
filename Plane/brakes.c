#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"

unsigned long in1 = 0;
unsigned long in2 = 0;

void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void)
{
	in1 = IC2BUF;
	in1 = (in1 << 8) | IC1BUF;
	_IC1IF = 0;
}	

void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void)
{
	IC3CON2bits.TRIGSTAT = 0;
	in2 = IC4BUF;
	in2 = (in2 << 8) | IC3BUF;
	IC3CON2bits.TRIGSTAT = 1;
	_IC3IF = 0;
}	


void initBrakes()
{
	//Input capture and output compare settings
	//reports whichever is the smaller of the counts
	TRISFbits.TRISF3 = 1;
	TRISFbits.TRISF2 = 1;
	
	RPINR7bits.IC1R = 16;
	RPINR8bits.IC3R = 30;
	RPOR11bits.RP23R = 18;
	RPOR12bits.RP24R = 19;
	
	//PWM1 Left setup
	OC1R = 25000;			//duty cycle of 50%
	OC1RS = 50000;			//period
	OC1CON1bits.OCTSEL = 0b111;
	OC1CON2bits.SYNCSEL = 0x1F;
	OC1CON1bits.OCM = 0b110;	//PWM, no fault
	//PWM2 Right setup
	OC2RS = 50000;
	OC2R = 25000;
	OC2CON1bits.OCTSEL = 0b111;
	OC2CON2bits.SYNCSEL = 0x1F;
	OC2CON1bits.OCM = 0b110;
	
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
