#include <p24FJ256GA110.h>
#include "../shared/status.h"
#include "../shared/LEDs.h"
#include "../shared/uart.h"
#include "../shared/buttons.h"
#include "gLCD.h"
#include "screens.h"

//Config Bits set here
//Reference p24FJ256GA110.h
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & COE_OFF  & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & IOL1WAY_OFF & FNOSC_FRCPLL)

extern unsigned char sw1LastState;
extern unsigned char sw2LastState;
extern unsigned char sw3LastState;

char st[] = "Hello";
int main(void)
{
	unsigned int i = 0;
	
	CLKDIVbits.RCDIV = 0b000;
	CLKDIVbits.DOZE = 0b000;
	INTCON1bits.NSTDIS = 1; //Disables interrupt nesting
	initLEDs();
	initStatus();
	initUart();
	initButtons();
	initLCD();
	lcd_screenon(1);
	lcd_cls();
	
	lcd_selectside(LEFT);
	lcd_setpage(0);
	lcd_setyaddr(4);
	lcd_write(0xFF);
	lcd_write(0xFF);
	lcd_write(0xFF);
	lcd_plotpixel(5,5);

	lcd_setpage(1);
	lcd_setyaddr(15);
	lcd_write(0xFF);
	lcd_write(0xFF);
	lcd_write(0xFF);
	
	lcd_setpage(3);
	lcd_setyaddr(0);
	lcd_putcharsmall('T');
	lcd_putcharsmall('a');
	lcd_putcharsmall('k');
	lcd_putcharsmall('e');
	lcd_putcharsmall('o');
	lcd_putcharsmall('f');
	lcd_putcharsmall('f');
	lcd_putcharsmall('=');
	lcd_putcharsmall('1');
	lcd_putcharsmall('3');
	lcd_setpage(4);
	lcd_setyaddr(0);
	lcd_puts(st);
	lcd_setpage(5);
	lcd_setyaddr(0);
	
	drawHome();
	
	while(1)
	{
		LED1 = !sw1LastState;
		LED2 = !sw2LastState;
		LED3 = !sw3LastState;
//		writeUart(1);
//		for(i=0; i<10; i++)
//		{
//			lcd_setyaddr(0);
//			lcd_putchar(i);
//		}	
	}	
}
