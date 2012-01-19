#ifndef PINS_H
#define PINS_H

void initPins(void);

//defines pins as human readable names

//Format of code (* represents any name of pin)
//* gives a name to the specified pin so that * can be set or read in the program
//* Tris sets the Tris bit for the pin. 0 is output, 1 is input.

//Status LEDs
#define LED1 PORTAbits.RA6
#define LED1Tris TRISAbits.TRISA6
#define LED2 PORTGbits.RG0
#define LED2Tris TRISGbits.TRISG0
#define LED3 PORTGbits.RG1
#define LED3Tris TRISGbits.TRISG1
#define LED4 PORTFbits.RF1
#define LED4Tris TRISFbits.TRISF1
#define LED5 PORTFbits.RF0
#define LED5Tris TRISFbits.TRISF0

#define LEDStatus PORTAbits.RA7
#define LEDStatusTris TRISAbits.TRISA7

#define button2 PORTGbits.RG12
#define button3 PORTGbits.RG13
#define button1 PORTGbits.RG14

#endif
