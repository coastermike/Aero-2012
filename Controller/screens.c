#include <p24FJ256GA110.h>
#include <stdio.h>
#include "glcd.h"
#include "screens.h"
#include "../shared/buttons.h"
#include "../shared/LEDs.h"

extern unsigned char sw1LastState;
extern unsigned char sw2LastState;
extern unsigned char sw3LastState;
extern unsigned char receive[];

char takeOffStr[20], takeOffStrUpd[6];
char landingStr[20], landingStrUpd[6];
char totalDistStr[20], totalDistStrUpd[6];
char debugTitle[10], settingsTitle[10], calibrateTitle[10], resetTitle[10];
char buttonHome[3], buttonDebug[3], buttonSettings[3], buttonReset[3], buttonCalibrate[3], buttonYes[3], buttonNo[3];
unsigned int takeoffraw = 0, landingraw = 0, totalDistInt = 0, totalDistDec = 0;
unsigned int takeoffInt = 0, landingInt = 0, takeoffDec = 0, landingDec = 0;

unsigned int screenState = 0;
unsigned int sw1Pressed = 0, sw2Pressed = 0, sw3Pressed = 0;

void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
	//button check
	sw1Pressed = swState(1);
	sw2Pressed = swState(2);
	sw3Pressed = swState(3);
	clearSW();
	switch (screenState)
	{
		case 0:	//presently home
			if(sw1Pressed)
			{
				drawDebug();
			}
			else if(sw2Pressed)
			{
				drawSettings();
			}
			else if(sw3Pressed)
			{
				
			}
			else
			{
				updateHome();
				takeoffraw++;
				landingraw = landingraw+3;
			}				
			break;
		case 1:	//presently debug
			if(sw1Pressed)
			{
				
			}
			else if(sw2Pressed)
			{
				drawSettings();
			}
			else if(sw3Pressed)
			{
				drawHome();
			}
			else
			{
				updateDebug();
			}
			break;
		case 2:	//presently calibrate
			if(sw1Pressed)
			{
				
			}
			else if(sw2Pressed)
			{
				
			}
			else if(sw3Pressed)
			{
				drawHome();
			}
			else
			{
				updateCalibrate();
			}
			break;
		case 3:	//presently reset
			if(sw1Pressed)
			{
				drawHome();
			}
			else if(sw2Pressed)
			{
				drawHome();
			}
			else if(sw3Pressed)
			{
				//reset
			}
			else
			{
				//updateReset();
			}
			break;
		case 4:	//presently Settings
			if(sw1Pressed)
			{
				drawReset();
			}
			else if(sw2Pressed)
			{
				drawCalibrate();
			}
			else if(sw3Pressed)
			{
				drawHome();
			}
			else
			{
				//updateSettings();
			}
			break;
		default:
			drawHome();
			break;
	}
	LED4=~LED4;
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
	_T4IF = 0;
	_T4IP = 3;
	_T4IE = 1;
	
	drawHome();
}

//because they seem to disappear if not remade before printing to screen even though global?!?
void createButtonLabels()
{
	sprintf(buttonDebug, "D>");
	sprintf(buttonHome, "H>");
	sprintf(buttonSettings, "S>");
	sprintf(buttonReset, "R>");
	sprintf(buttonCalibrate, "C>");
	sprintf(buttonYes, "Y>");
	sprintf(buttonNo, "N>");
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
	sprintf(takeOffStr, "Takeoff: %d.%d ft ", takeoffInt, takeoffDec);
	lcd_string(0, 2, takeOffStr, 1);
	sprintf(landingStr, "Landing: %d.%d ft ", landingInt, landingDec);
	lcd_string(0, 3, landingStr, 1);
	sprintf(totalDistStr, "  Total: %d.%d ft ", totalDistInt, totalDistDec);
	lcd_string(0, 4, totalDistStr, 1);
	
	createButtonLabels();
	lcd_string(121, 2, buttonDebug, 0);
	lcd_string(121, 5, buttonSettings, 0);
//	lcd_string(121, 7, buttonHome, 0);
}

void updateHome()
{
	calculateFeet();
	sprintf(takeOffStrUpd, "%d.%d ft ", takeoffInt, takeoffDec);
	lcd_string(54, 2, takeOffStrUpd, 1);
	sprintf(landingStrUpd, "%d.%d ft ", landingInt, landingDec);
	lcd_string(54, 3, landingStrUpd, 1);
	sprintf(totalDistStrUpd, "%d.%d ft ", totalDistInt, totalDistDec);
	lcd_string(54, 4, totalDistStrUpd, 1);
}

void drawDebug()
{
	screenState = 1;
	lcd_cls();
	calculateFeet();
	sprintf(debugTitle, "DEBUG");
	lcd_string(0,0, debugTitle, 0);
	
	sprintf(takeOffStrUpd, "%d", receive[4]);
	lcd_string(0, 1, takeOffStrUpd, 0);
	
	sprintf(takeOffStrUpd, "%d.%d", takeoffInt, takeoffDec);
	lcd_string(0, 2, takeOffStrUpd, 0);
	
	createButtonLabels();
//	lcd_string(121, 2, buttonDebug, 0);
	lcd_string(121, 5, buttonSettings, 0);
	lcd_string(121, 7, buttonHome, 0);
}

void updateDebug()
{
	sprintf(takeOffStrUpd, "%d", receive[4]);
	lcd_string(0, 1, takeOffStrUpd, 0);
}

void drawCalibrate()
{
	screenState = 2;
	lcd_cls();
	
	sprintf(calibrateTitle, "CALIBRATION");
	lcd_string(0,0, calibrateTitle, 1);
	
	createButtonLabels();
//	lcd_string(121, 2, buttonDebug, 0);
//	lcd_string(121, 5, buttonSettings, 0);
	lcd_string(121, 7, buttonHome, 0);
}

void updateCalibrate()
{
	
}

void drawReset()
{
	screenState = 3;
	lcd_cls();
	
	sprintf(resetTitle, "RESET-WARNING");
	lcd_string(0,0, resetTitle, 0);
	
	createButtonLabels();
	lcd_string(121, 2, buttonNo, 0);
	lcd_string(121, 5, buttonNo, 0);
	lcd_string(121, 7, buttonYes, 0);
}

void drawSettings()
{
	screenState = 4;
	lcd_cls();
	
	sprintf(settingsTitle, "SETTINGS");
	lcd_string(0,0, settingsTitle, 0);
	
	createButtonLabels();
	lcd_string(121, 2, buttonReset, 0);
	lcd_string(121, 5, buttonCalibrate, 0);
	lcd_string(121, 7, buttonHome, 0);
}
