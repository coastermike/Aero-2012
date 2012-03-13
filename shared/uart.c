#include <p24FJ256GA110.h>
#include "../shared/LEDs.h"
#include "../shared/uart.h"

#define PLANE

#define MAXTRANSMIT 29

extern unsigned int takeoff;
extern unsigned int landing;
extern unsigned int leftWheelTakeoff;
extern unsigned int rightWheelTakeoff;
extern unsigned int leftWheelLanding;
extern unsigned int rightWheelLanding;
extern unsigned int wowL;
extern unsigned int wowR;
extern unsigned int wowCal;
extern unsigned int IR;
extern unsigned int brakeL;
extern unsigned int brakeR;
extern unsigned int mode;
extern unsigned int accelX;
extern unsigned int accelY;
extern unsigned int accelZ;

unsigned char received = 0;
unsigned char previous = 0;
unsigned char transmit[MAXTRANSMIT];
unsigned char transmitCount = 0;
unsigned char receive[MAXTRANSMIT];
unsigned char receiveCount = 0;

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt (void)
{
	writeUart();
	LED5 = ~LED5;
	_T2IF = 0;
}

//U1RX interrupt
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt (void)
{
	_U1RXIF = 0;
	received = U1RXREG;
	
	#ifdef CONTROLLER
	if(receiveCount<3)
	{
		if((received == 0x40) && (receiveCount == 0))
		{
			receive[receiveCount] = received;
			receiveCount++;
		}
		else if((received == 0x30) && (receiveCount == 1))
		{
			receive[receiveCount] = received;
			receiveCount++;
		}
		else if((received == 0x20) && (receiveCount == 2))
		{
			receive[receiveCount] = received;
			receiveCount++;
		}
		else if((received == 0x10) && (receiveCount == 3))
		{
			receive[receiveCount] = received;
			receiveCount++;
		}
		else
		{
			receiveCount = 0;
		}
	}
	else if(receiveCount < MAXTRANSMIT)
	{
		receive[receiveCount] = received;
		receiveCount++;
	}
	else
	{
		receiveCount = 0;
	}
	if(receiveCount == MAXTRANSMIT)
	{
		takeoff = (receive[4]<<8) | receive[5];
		landing = (receive[6]<<8) | receive[7];
		leftWheelTakeoff = (receive[8]<<8) | receive[9];
		rightWheelTakeoff = (receive[10]<<8) | receive[11];
		leftWheelLanding = (receive[12]<<8) | receive[13];
		rightWheelLanding = (receive[14]<<8) | receive[15];
		wowL = receive[16];
		wowR = receive[17];
		wowCal = receive[18];
		IR = receive[19];
		brakeL = receive[20];
		brakeR = receive[21];
		mode = receive[22];
		accelX = receive[23];
		accelY = receive[24];
		accelZ = receive[25];
		receiveCount = 0;
	}
	#endif
}

//U1TX interrupt
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt (void)
{
	if(transmitCount < MAXTRANSMIT)
	{
		U1TXREG = transmit[transmitCount];
		transmitCount++;
	}	
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
	_U1RXIE = 1;
	_U1ERIE = 1;
	receiveCount = 0;
}
	
void initUart()
{
	unsigned char i = 0;
	
	RPINR18bits.U1RXR = 26;		//U1RX on RP26
	RPOR10bits.RP21R = 3;		//U1TX on RP21
	
	for(i=0; i<MAXTRANSMIT; i++)
	{
		transmit[i] = 0;
	}
	
	transmit[0] = 0x40;	//first 4 are a header that should indicate the start of transmission
	transmit[1] = 0x30;
	transmit[2] = 0x20;
	transmit[3] = 0x10;
		
	U1MODEbits.BRGH = 1;
	U1BRG = 68;					//baud rate generator value for 57600
	U1MODEbits.PDSEL = 0b00;	//8-bit data, no parity
	U1MODEbits.STSEL = 0;		//1 stop bits
	_U1RXIE = 1;				//enable rx interrupt
	U1STAbits.URXISEL = 0b00;	//interrupt when any char is received
	U1STAbits.UTXISEL1 = 0;	//interrupt ever transfer, at least one in buffer.
	U1STAbits.UTXISEL0 = 0;
	_U1RXIF = 0;				// reset RX flag
	_U1ERIE = 1;
	_U1RXIP = 5;				//priority 5
	_U1TXIF = 0;
	_U1TXIE = 1;
	_U1TXIP = 5;
	U1MODEbits.UARTEN = 1;		//enable uart
	U1STAbits.UTXEN = 1;		//enable uart
	
	#ifdef PLANE
	T2CONbits.T32 = 0;
	T2CONbits.TCKPS = 0b10;	//1:64 prescalar
	T2CONbits.TCS = 0;
	T2CONbits.TGATE = 0;
	PR2 = 12500;				//50ms timing
	T2CONbits.TON = 1;
	_T2IE = 1;
	_T2IP = 4;
	#endif
}

//store variables into a matrix of 28 variables. Then start transmit of 28 bytes.
void writeUart()
{
	transmit[4] = (char)(takeoff>>8);//takeoff;
	transmit[5] = (char)takeoff;//takeoff;
	transmit[6] = (char)(landing>>8);//landing;
	transmit[7] = (char)landing;//landing;
	transmit[8] = (char)(leftWheelTakeoff>>8);
	transmit[9] = (char)(leftWheelTakeoff);
	transmit[10] = (char)(rightWheelTakeoff>>8);
	transmit[11] = (char)(rightWheelTakeoff);
	transmit[12] = (char)(leftWheelLanding>>8);
	transmit[13] = (char)(leftWheelLanding);
	transmit[14] = (char)(rightWheelLanding>>8);
	transmit[15] = (char)(rightWheelLanding);
	transmit[16] = wowL;//wowL;
	transmit[17] = wowR;//wowR;
	transmit[18] = wowCal;//wowCal;
	transmit[19] = IR;//IR;
	transmit[20] = brakeL;
	transmit[21] = brakeR;
	transmit[22] = mode;
	transmit[23] = accelX;
	transmit[24] = accelY;
	transmit[25] = accelZ;
	transmit[26] = 0;
	transmit[27] = 0;
	transmit[28] = 20;
	
	while(!U1STAbits.TRMT);
	U1TXREG = transmit[0];
	U1TXREG = transmit[1];
	
	transmitCount = 2;
}
