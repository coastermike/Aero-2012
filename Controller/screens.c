#include <p24FJ256GA110.h>
#include <stdio.h>
#include "glcd.h"
#include "screens.h"
#include "../shared/buttons.h"

extern unsigned char sw1LastState;
extern unsigned char sw2LastState;
extern unsigned char sw3LastState;

char takeOffStr[20], takeOffStrUpd[6];
char landingStr[20], landingStrUpd[6];
char totalDistStr[20], totalDistStrUpd[6];
char buttonHome[3], buttonDebug[3], buttonSettings[3];
unsigned int takeoffraw = 4912, landingraw = 4102, totalDistInt = 0, totalDistDec = 0;
unsigned int takeoffInt = 0, landingInt = 0, takeoffDec = 0, landingDec = 0;

unsigned int screenState = 0;

void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
	//button check
	switch (screenState)
	{
		case 0:	//presently home
			if(!sw1LastState)
			{
				
			}
			else if(!sw2LastState)
			{
				
			}
			else if(!sw3LastState)
			{
				
			}
			else
			{
				updateHome();
			}				
			break;
		case 1:	//presently debug
			if(!sw1LastState)
			{
				
			}
			else if(!sw2LastState)
			{
				
			}
			else if(!sw3LastState)
			{
				
			}
			else
			{
				updateDebug();
			}
			break;
		case 2:	//presently calibrate
			if(!sw1LastState)
			{
				
			}
			else if(!sw2LastState)
			{
				
			}
			else if(!sw3LastState)
			{
				
			}
			else
			{
				updateCalibrate();
			}
			break;
		case 3:	//presently reset
			if(!sw1LastState)
			{
				
			}
			else if(!sw2LastState)
			{
				
			}
			else if(!sw3LastState)
			{
				
			}
			else
			{
				//updateReset();
			}
			break;
		case 4:	//presently Settings
			if(!sw1LastState)
			{
				
			}
			else if(!sw2LastState)
			{
				
			}
			else if(!sw3LastState)
			{
				
			}
			else
			{
				//updateSettings();
			}
		default:
			drawHome();
			break;
	}		
	//screen update
	updateHome();
	_T4IF = 0;
}

void initScreens()
{
	//timer for screen update
	T4CONbits.T32 = 0;
	T4CONbits.TCKPS = 0b10;	//1:64 prescalar
	T4CONbits.TCS = 0;
	T4CONbits.TGATE = 0;
	PR4 = 25000;				//100ms timing
	T4CONbits.TON = 1;
	
	drawHome();
}
	
void calculateFeet()
{
	takeoffInt = takeoffraw/20;
	takeoffDec = (takeoffraw%20)*5;
	landingInt = landingraw/20;
	landingDec = (landingraw%20)*5;
	if((takeoffDec + landingDec) > 99)
	{
		totalDistDec = (takeoffDec + landingDec) - 100;
		totalDistInt = takeoffInt + landingInt + 1;
	}
	else
	{	
		totalDistInt = takeoffInt + landingInt;
		totalDistDec = takeoffDec + landingDec;
	}	
}
	
void drawHome()
{
	screenState = 0;
	lcd_cls();
	calculateFeet();
	sprintf(takeOffStr, "Takeoff: %d.%d ft", takeoffInt, takeoffDec);
	lcd_string(0, 2, takeOffStr, 1);
	sprintf(landingStr, "Landing: %d.%d ft", landingInt, landingDec);
	lcd_string(0, 3, landingStr, 1);
	sprintf(totalDistStr, "  Total: %d.%d ft", totalDistInt, totalDistDec);
	lcd_string(0, 4, totalDistStr, 1);
	
	sprintf(buttonDebug, "D>");
	sprintf(buttonHome, "H>");
	sprintf(buttonSettings, "S>");
	lcd_string(121, 2, buttonDebug, 0);
	lcd_string(121, 5, buttonSettings, 0);
	lcd_string(121, 7, buttonHome, 0);
}

void updateHome()
{
	calculateFeet();
	sprintf(takeOffStrUpd, "%d.%d ft", takeoffInt, takeoffDec);
	lcd_string(54, 2, takeOffStrUpd, 1);
	sprintf(landingStrUpd, "%d.%d ft", landingInt, landingDec);
	lcd_string(54, 3, landingStrUpd, 1);
	sprintf(totalDistStrUpd, "%d.%d ft", totalDistInt, totalDistDec);
	lcd_string(54, 4, totalDistStrUpd, 1);
}

void drawDebug()
{
	screenState = 1;
	lcd_cls();
	calculateFeet();
	sprintf(takeOffStrUpd, "%d.%d", takeoffInt, takeoffDec);
	lcd_string(9, 2, takeOffStrUpd, 1);
}

void updateDebug()
{
	
}

void drawCalibrate()
{
	screenState = 2;
}

void updateCalibrate()
{
	
}

void drawReset()
{
	screenState = 3;
}

void drawSettings()
{
	screenState = 4;
}
