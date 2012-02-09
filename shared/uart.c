#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"



unsigned char received = 0;
unsigned char previous = 0;
unsigned char transmit[28];

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
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt (void)
{
	_U1TXIF = 0;
}
	
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
	unsigned char i = 0;
	
	RPINR18bits.U1RXR = 26;		//U1RX on RP26
	RPOR10bits.RP21R = 3;		//U1TX on RP21
	
	for(i=0; i<28; i++)
	{
		transmit[i] = 0;
	}
	
	transmit[0] = 0x40;	//first 4 are a header that should indicated the start of transmission
	transmit[1] = 0x30;
	transmit[2] = 0x20;
	transmit[3] = 0x10;
		
	U1MODEbits.BRGH = 1;
	U1BRG = 68;					//baud rate generator value for 57600
	U1MODEbits.PDSEL = 0b00;	//8-bit data, no parity
	U1MODEbits.STSEL = 0;		//1 stop bits
	_U1RXIE = 1;				//enable rx interrupt
	U1STAbits.URXISEL = 0b00;	//interrupt when any char is received
	_U1RXIF = 0;				// reset RX flag
	_U1ERIE = 0;
	_U1RXIP = 5;				//priority 5
	_U1TXIF = 0;
	_U1TXIE = 1;
	_U1TXIP = 5;
	U1MODEbits.UARTEN = 1;		//enable uart
	U1STAbits.UTXEN = 1;		//enable uart
}

//store variables into a matrix of 28 variables. Then start transmit of 28 bytes.
void writeUart()
{
	transmit[4] = takeoff;
	transmit[5] = takeoff;
	transmit[6] = landing;
	transmit[7] = landing;
	transmit[8] = leftWheelTakeoff;
	transmit[9] = leftWheelTakeoff;
	transmit[10] = rightWheelTakeoff;
	transmit[11] = rightWheelTakeoff;
	transmit[12] = leftWheelLanding;
	transmit[13] = leftWheelLanding;
	transmit[14] = rightWheelLanding;
	transmit[15] = rightWheelLanding;
	transmit[16] = wowL;
	transmit[17] = wowR;
	transmit[18] = wowCal;
	transmit[19] = IR;
	transmit[20] = 0;
	transmit[21] = 0;
	transmit[22] = 0;
	transmit[23] = 0;
	transmit[24] = 0;
	transmit[25] = 0;
	transmit[26] = 0;
	transmit[27] = 0;
	transmit[28] = 0;
	
	while(U1STAbits.UTXBF);
	U1TXREG = transmit[0];
}
