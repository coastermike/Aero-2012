#ifndef LEDS_H
#define LEDS_H

void initLEDs(void);

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

#endif
