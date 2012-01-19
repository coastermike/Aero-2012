#include <p24FJ256GA110.h>
#include "../shared/pins.h"

void initUart()
{
	RPINR18bits.U1RXR = 26;
	RPOR10bits.RP21R = 3;
	U1MODEbits.BRGH = 1;
	U1BRG = 15;
	U1MODEbits.PDSEL = 0b00;	//8-bit data, no parity
	U1MODEbits.STSEL = 1;		//2 stop bits
	_U1RXIE = 0;				//enable rx interrupt
	U1STAbits.URXISEL = 0b00;	//interrupt when any char is received
	U1MODEbits.UARTEN = 1;		//enable uart
	_U1ERIE = 0;
	_U1RXIF = 0;				// reset RX flag
	_U1RXIP = 0b101;			//priority 5
}
