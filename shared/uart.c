#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"

unsigned char received = 0;
unsigned char previous = 0;

//U1RX interrupt
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt (void)
{
	_U1RXIF = 0;
	received = U1RXREG;
	if(received == 1)
	{
		LED1 = 1;
		Nop();
		LED2 = 0;
		Nop();
		LED3 = 0;
	}
	else if(received == 2)
	{
		LED1 = 0;
		Nop();
		LED2 = 1;
		Nop();
		LED3 = 0;
	}
	else if(received == 3)
	{
		LED1 = 0;
		Nop();
		LED2 = 0;
		Nop();
		LED3 = 1;
	}
	else
	{
		LED1 = 0;
		Nop();
		LED2 = 0;
		Nop();
		LED3 = 0;
	}
}

//U1TX interrupt
//void __attribute__(interrupt, no_auto_psv)) _U1TXInterrupt (void)
//{
//	_U1TXIF = 0;
//}
	
//UART error interrupt
//NEED figure out which error occured. 
void __attribute__((interrupt, no_auto_psv)) _U1ErrInterrupt (void)
{
	_U1ERIF = 0;
	if(U1STAbits.OERR)
	{
		_U1RXIE = 0;
		_U1ERIE = 0;
		U1STAbits.OERR=0;
	}
	if(U1STAbits.FERR)
	{
		
	}
	if(U1STAbits.PERR)
	{
		
	}
}
	
void initUart()
{
	RPINR18bits.U1RXR = 26;		//U1RX on RP26
	RPOR10bits.RP21R = 3;		//U1TX on RP21
	
	U1MODEbits.BRGH = 1;
	U1BRG = 68;					//baud rate generator value for 57600
	U1MODEbits.PDSEL = 0b00;	//8-bit data, no parity
	U1MODEbits.STSEL = 0;		//1 stop bits
	_U1RXIE = 1;				//enable rx interrupt
	U1STAbits.URXISEL = 0b00;	//interrupt when any char is received
	_U1ERIE = 0;
	_U1RXIF = 0;				// reset RX flag
	_U1RXIP = 0b101;			//priority 5
	U1MODEbits.UARTEN = 1;		//enable uart
	U1STAbits.UTXEN = 1;		//enable uart
}

void writeUart(unsigned char data)
{
	while(U1STAbits.UTXBF);
	U1TXREG = data;
}
