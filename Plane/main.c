#include <p24FJ256GA110.h>

//Config Bits set here
//Reference p24FJ256GA110.h
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & COE_OFF  & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & IOL1WAY_OFF & FNOSC_FRCPLL)

#define LEDStatus PORTGbits.RG15
#define LEDStatusTris TRISGbits.TRISG15
#define LED1 PORTDbits.RD11
#define LED1Tris TRISDbits.TRISD11
#define LED3 PORTBbits.RB6
#define LED3Tris TRISBbits.TRISB6
#define STEPL PORTFbits.RF2
#define STEPR PORTDbits.RD2

unsigned int pins = 0;

//Status LED, 1sec period
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
	LEDStatus=~LEDStatus;
	STEPR = ~STEPR;
	STEPL = ~STEPL;
	IFS1bits.T5IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void)
{
	pins++;
	LED1=~LED1;
	IFS1bits.INT1IF = 0;
}

int main(void)
{	
	INTCON1bits.NSTDIS = 1;
	LEDStatusTris = 0;
	LED1Tris = 0;
	LED3Tris = 0;
	TRISAbits.TRISA12 = 1;
	TRISFbits.TRISF2 = 0;
	TRISDbits.TRISD2 = 0;
	
	AD1PCFGH = 0xFFFF;
	AD1PCFGL = 0xFFFF;
	
	//setup timer5 for status LED
	T5CONbits.TON = 0;
	T5CONbits.TCS = 0;
	T5CONbits.TGATE = 0;
	T5CONbits.TCKPS = 0b11;	//prescaler 1:256
	TMR5 = 0x00;
	PR5 = 0x0010;	//Value to generate interrupt - 0.5sec
	
	IPC7bits.T5IP = 2;
	IFS1bits.T5IF = 0;
	IEC1bits.T5IE = 1;
	
	IPC5bits.INT1IP = 3;
	IFS1bits.INT1IF = 0;
	INTCON2bits.INT1EP = 0; //pos edge
	IEC1bits.INT1IE = 1;
	
	T5CONbits.TON = 1;
	
	while(1)
	{
		if(PORTAbits.RA12 == 1)
		{
			LED3=1;
		}	
		else
		{
			LED3 = 0;
		}	
	}		
}	
