#include <p24FJ256GA110.h>
#include "glcd.h"

//CS1 writes right half screen
//CS2 writes left half screen
//RST needs to be high

void msDelay(unsigned int ms)
{
	unsigned long i, temp;
	temp = (long)333*ms;
	for(i=0; i<(temp);i++);
	i=0;
}

void initLCD()
{
	TRISBbits.TRISB15 = 0;
	TRISDbits.TRISD10 = 0;
	TRISDbits.TRISD11 = 0;
	TRISDbits.TRISD3 = 0;
	TRISDbits.TRISD4 = 0;
	TRISDbits.TRISD5 = 0;
	TRISGbits.TRISG8 = 0;
	
	PMCON = 0b1000001100000011;
	PMMODE = 0b0000001100010011;
	PMAEN = 0x0001;
	
	RESET = 1;
	
/*	T2CONbits.T32 = 0;					//SEEMS TO CRASH WITH THIS IN CODE??!?
	T2CONbits.TCKPS = 0b00;	//1:1 prescalar
	T2CONbits.TCS = 0;
	T2CONbits.TGATE = 0;
	PR2 = 16;				//1us timing
	T2CONbits.TON = 1;
	_T2IE=1;*/
}		
	
void setBackLightOn()
{
	BACKLIGHT = 1;
}

void setBackLightOff()
{
	BACKLIGHT = 0;
}		

void _lcd_reset(void)
{
	// reset the lcd module
    // datasheet says reset must be low for minimum of 1us
	// after Vdd clears 4.5v.
	// from experimentation, this is bullshit. this seems to
    // work though.
	msDelay(1); // actually .5 ms
	RESET=0;
	msDelay(1000); // actually .5 ms
	RESET=1;

	// check status, and wait if necessary

	while (_lcd_status() & 0b00010000);

}

// turn the display on or off
void lcd_screenon(unsigned char on)
{
	msDelay(2000);
	RW=0;
	PMADDR = 0;
	PMDIN1 = 0b00111110 | on;
	msDelay(2000);
}

void lcd_cls(void)
{
	unsigned char i,x,y;
	for(i=1; i<3; i++)
	{
		lcd_selectside(i);
		for (x=0; x<8; x++)
		{
			// set the page (x)
			lcd_setpage(x);
			// set the y address to 0
			lcd_setyaddr(0);
			// clear the row
			for (y=0; y<64; y++)
			{
				lcd_write(0);
			}
		}
	}	
}

void lcd_setpage(unsigned char page)
{
	_lcd_waitbusy();
	RW=0;
	PMADDR = 0;
	PMDIN1 = 0b10111000 | page;
}

void lcd_setyaddr(unsigned char y)
{
    _lcd_waitbusy();
	RW=0;
	PMADDR=0;
	PMDIN1 = 0b01000000 | (y & 0b00111111);
}

void _lcd_waitbusy(void)
{
    while (_lcd_status() & 0b10000000);
}

unsigned char _lcd_status(void)
{
	// returns the lcd status
	unsigned char _status = 0;

	// read the status
	RW=1;
	PMADDR=0;
	while(PMMODEbits.BUSY);
	_status = PMDIN1;
	while(PMMODEbits.BUSY);
	_status = PMDIN1;
	Nop();
	return _status;
}

void lcd_write (unsigned char data)
{
	_lcd_waitbusy();
	RW=0;
	PMADDR=1;
	PMDIN1 = data;
}

void lcd_selectside(unsigned char sides)
{
	// set a CS pin low to enable it
	if (sides & LEFT)
		CS1 = 0;
	else
		CS1 = 1;
	if (sides & RIGHT)
		CS2 = 0;
	else
		CS2 = 1;
}

unsigned char lcd_read (void)
{
	unsigned char _data;
	RW = 1;
	PMADDR = 1;
	_data = PMDIN1;
	_data = PMDIN1;
	return _data;
}

void lcd_plotpixel(unsigned char rx, unsigned char ry)
{
	unsigned char data;
	_lcd_waitbusy();
	// select the correct side
	if (rx & 64)
	lcd_selectside(RIGHT);
	else
	lcd_selectside(LEFT);
	 
	lcd_setpage( ry >> 3);
	lcd_setyaddr( rx & 0b00111111);
	data = lcd_read(); // dummy read needed here
	data = lcd_read();
	lcd_setyaddr( rx & 0b00111111);
	lcd_write (data | (1 << (ry & 0b111)));
}

void lcd_string(unsigned char page, unsigned char yaddress, char *string, unsigned char font)
{
	lcd_setpage(page);
	lcd_setyaddr(yaddress);
	if(font)
	{
		lcd_puts(string);
	}
	else
	{
		lcd_putsmalls(string);
	}		
}
	
void lcd_putchar(char c)
{
	int base;
	base = c - 32;
	base *= 5;	//CHANGED FROM 3 HOPEFULLY TO FIX font size
	lcd_write(font[base]);
	lcd_write(font[base + 1]);
	lcd_write(font[base + 2]);
	lcd_write(font[base + 3]);	//ADDED
	lcd_write(font[base + 4]);	//ADDED
	lcd_write(0);
}

void lcd_putcharsmall(char c)
{
	int base;
	base = c - 32;
	base *= 4;	//CHANGED FROM 3 HOPEFULLY TO FIX font size
	lcd_write(fontsmall[base]);
	lcd_write(fontsmall[base + 1]);
	lcd_write(fontsmall[base + 2]);
	lcd_write(fontsmall[base + 3]);	//ADDED
}

void lcd_puts(char *string)
{
	unsigned char i=0;
	while (string[i] != 0)
	lcd_putchar(string[i++]);
}

void lcd_putsmalls(char *string)
{
	unsigned char i=0;
	while (string[i] != 0)
	lcd_putcharsmall(string[i++]);
}
