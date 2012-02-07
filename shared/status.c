#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
	_T1IF = 0;
	LEDStatus = ~LEDStatus;
}	

void initStatus(void) //set up timers and interrupts
{
	T1CONbits.TON = 1;
	T1CONbits.TCKPS = 0b11;
	T1CONbits.TCS = 0;
	T1CONbits.TGATE = 0;
	PR1 = 10000;//31250;
	_T1IE = 1;
	_T1IP = 2;
}
