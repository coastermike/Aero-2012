#include <p24FJ256GA110.h>
#include "../shared/status.h"
#include "../shared/LEDs.h"
#include "../shared/uart.h"
#include "../shared/buttons.h"
#include "wheels.h"
#include "brakes.h"
#include "accel.h"
#include "analog.h"

//Config Bits set here
//Reference p24FJ256GA110.h
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & COE_OFF  & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & IOL1WAY_OFF & FNOSC_FRCPLL)

unsigned int stateOf = 0, ground = 0;

extern unsigned int wowL, wowR, calWowL, calWowR, IR;
extern unsigned int tempLeftCount, tempRightCount;
extern unsigned int calGndComplete, calAirComplete;

extern unsigned int mode;

int main(void)
{	
	CLKDIVbits.RCDIV = 0b000;
	CLKDIVbits.DOZE = 0b000;
	INTCON1bits.NSTDIS = 1; //Disables interrupt nesting
	initLEDs();
	initAnalog();
	initStatus();
	initUart();
	initButtons();
	initWheels();
	initBrakes();
	initAccel();
	
	OC1R = 0;
	OC2R = 0;
	
	//timer4 for software debounce for runway bounces
	T4CONbits.T32 = 0;
	T4CONbits.TCKPS = 0b11;	//1:64 prescalar
	T4CONbits.TCS = 0;
	T4CONbits.TGATE = 0;
	PR4 = 31250;				//0.5s timing
//	T4CONbits.TON = 1;
	
	while(1)
	{
		if(CMSTATbits.C2OUT)//IR
		{
			LED2= 1;
			IR=1;
			ground = 1;
		}
		else
		{
			LED2= 0;
			IR=0;
			ground = 0;
		}
		
		//test code to make LEDs blink to hall effects
		if(rightHallTop)
		{
			LED1 = 1;
		}
		else
		{
			LED1 = 0;
		}
		if(rightHallBottom)
		{
//			LED2 = 1;
		}
		else
		{
//			LED2 = 0;
		}
		//end hall effect test code
		
		//run analog to get wow readings
		if(stateOf != 42)
		{
			AD1CON1bits.ASAM = 1;
			while(AD1CON1bits.ASAM && _AD1IF);
		}
		//
		//state machine start here
		//
		//waiting for cal
//		if(stateOf == 42)
//		{
//			if((calGndComplete == 1) && (calAirComplete == 1))
//			{
//				stateOf = 43;
//			}
//		}
//		else if(stateOf == 43)
//		{
//			
//		}	
		//on runway for takeoff	
		if(stateOf == 0)
		{
			if(ground == 0)	//if((wowL > 190) || (wowR > 190)) //wheels lift
			{
				stateOf = 1;
			}
		}
		else if(stateOf == 1)
		{
			T4CONbits.TON = 1;
			stateOf = 2;
		}
		//check for retouch in 0.5seconds
		else if(stateOf == 2)
		{
			if(ground == 1)//if(((wowL < 190) && (wowL > 0)) || ((wowR < 190) && (wowR > 0)))//wheel touches
			{
				stateOf = 0;
				T4CONbits.TON = 0;
				//add temp count to full since still takeoff
			}	
			else if(_T4IF)
			{
				T4CONbits.TON = 0;
				_T4IF = 0;
				tempLeftCount = 0;
				tempRightCount = 0;
				stateOf = 3;
			}
		}
		//looking for landing, state set from timer interrupting
		else if(stateOf == 3)
		{
			if(ground == 1)//if(((wowL < 190) && (wowL > 0)) || ((wowR < 190) && (wowR > 0)))//wheel touches
			{
				stateOf = 4;
			}
		}
		else if(stateOf == 4)
		{
			T4CONbits.TON = 1;
			stateOf = 5;
		}
		//landed saving to temp to verify
		else if(stateOf == 5)
		{
			if(ground == 0)//if((wowL > 190) || (wowR > 190)) //wheels lift
			{
				stateOf = 3;
				T4CONbits.TON = 0;
			}
			else if(_T4IF)
			{
				T4CONbits.TON = 0;
				_T4IF = 0;//
				tempLeftCount = 0;
				tempRightCount = 0;
				stateOf = 6;
			}
		}
		//actually landing, set from timer interrupt
		else if(stateOf == 6)
		{
			
		}			

		mode = stateOf;
	}		
}	
