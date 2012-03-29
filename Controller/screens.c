#include <p24FJ256GA110.h>
#include <stdio.h>
#include "glcd.h"
#include "screens.h"
#include "../shared/buttons.h"
#include "../shared/LEDs.h"

extern unsigned char sw1LastState;
extern unsigned char sw2LastState;
extern unsigned char sw3LastState;

unsigned int takeoff = 0;
unsigned int landing = 0;
unsigned int leftWheelTakeoff = 0;
unsigned int rightWheelTakeoff = 0;
unsigned int leftWheelLanding = 0;
unsigned int rightWheelLanding = 0;
unsigned int wowL = 0;
unsigned int wowR = 0;
unsigned int wowCal = 0;
unsigned int IR = 0;
unsigned int brakeL = 0;
unsigned int brakeR = 0;
unsigned int mode = 0;
unsigned int accelX = 0;
unsigned int accelY = 0;
unsigned int accelZ = 0;

char takeOffStr[20], takeOffStrUpd[6];
char landingStr[20], landingStrUpd[6];
char totalDistStr[20], totalDistStrUpd[6];
char wheelTakeoffStr[20], wheelTakeoffStrUpd[10];
char wheelLandingStr[20], wheelLandingStrUpd[10];
char wowLStr[20], wowLStrUpd[6], wowRStr[20], wowRStrUpd[6], wowCalStr[20], wowCalStrUpd[6], IRStr[20], IRStrUpd[6];
char brakeLStr[20], brakeLStrUpd[6], brakeRStr[20], brakeRStrUpd[6];
char modeStr[20], modeStrUpd[6];
char accelXStr[20], accelXStrUpd[6], accelYStr[20], accelYStrUpd[6], accelZStr[20], accelZStrUpd[6];
char debugTitle[10], settingsTitle[10], calibrateTitle[10], resetTitle[10];
char buttonHome[3], buttonDebug[3], buttonSettings[3], buttonReset[3], buttonCalibrate[3], buttonYes[3], buttonNo[3];
unsigned int totalDistInt = 0, totalDistDec = 0;
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
	takeoffInt = takeoff/20;
	takeoffDec = (takeoff%20)*5;	
	landingInt = landing/20;
	landingDec = (landing%20)*5;
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
	if(takeoffDec == 5 || takeoffDec == 0)
	{
		sprintf(takeOffStr, "Takeoff: %u.0%u ft ", takeoffInt, takeoffDec);
	}
	else	
	{
		sprintf(takeOffStr, "Takeoff: %u.%u ft ", takeoffInt, takeoffDec);
	}	
	lcd_string(0, 2, takeOffStr, 1);
	if(landingDec == 5 || landingDec == 0)
	{
		sprintf(landingStr, "Landing: %u.0%u ft ", landingInt, landingDec);
	}
	else
	{
		sprintf(landingStr, "Landing: %u.%u ft ", landingInt, landingDec);
	}		
	lcd_string(0, 3, landingStr, 1);
	if(totalDistDec == 5 || totalDistDec == 0)
	{
		sprintf(totalDistStr, "  Total: %u.0%u ft ", totalDistInt, totalDistDec);
	}
	else
	{
		sprintf(totalDistStr, "  Total: %u.%u ft ", totalDistInt, totalDistDec);
	}		
	lcd_string(0, 4, totalDistStr, 1);
	
	createButtonLabels();
	lcd_string(121, 2, buttonDebug, 0);
	lcd_string(121, 5, buttonSettings, 0);
//	lcd_string(121, 7, buttonHome, 0);
}

void updateHome()
{
	calculateFeet();
	if(takeoffDec == 5 || takeoffDec == 0)
	{
		sprintf(takeOffStrUpd, "%u.0%u ft ", takeoffInt, takeoffDec);
	}
	else	
	{
		sprintf(takeOffStrUpd, "%u.%u ft ", takeoffInt, takeoffDec);
	}	
	lcd_string(54, 2, takeOffStrUpd, 1);
	if(landingDec == 5 || landingDec == 0)
	{
		sprintf(landingStrUpd, "%u.0%u ft ", landingInt, landingDec);
	}
	else
	{
		sprintf(landingStrUpd, "%u.%u ft ", landingInt, landingDec);
	}	
	lcd_string(54, 3, landingStrUpd, 1);
	if(totalDistDec == 5 || totalDistDec == 0)
	{
		sprintf(totalDistStrUpd, "%u.0%u ft ", totalDistInt, totalDistDec);
	}
	else
	{
		sprintf(totalDistStrUpd, "%u.%u ft ", totalDistInt, totalDistDec);
	}	
	lcd_string(54, 4, totalDistStrUpd, 1);
}

void drawDebug()
{
	screenState = 1;
	lcd_cls();
	calculateFeet();
	sprintf(debugTitle, "DEBUG");
	lcd_string(0,0, debugTitle, 0);
	
	sprintf(takeOffStr, "Takeoff: %4u", takeoff);
	lcd_string(0, 1, takeOffStr, 0);
	sprintf(landingStr, "Landing: %4u", landing);
	lcd_string(0, 2, landingStr, 0);
	
	sprintf(wowLStr, "wowL: %2u", wowL);
	lcd_string(0,3, wowLStr, 0);
	sprintf(wowRStr, "wowR: %2u", wowR);
	lcd_string(0,4, wowRStr, 0);
	sprintf(wowCalStr, "wowC: %2u", wowCal);
	lcd_string(0,5, wowCalStr, 0);
	sprintf(IRStr, "IR: %2u", IR);
	lcd_string(0,6, IRStr, 0);
	
	sprintf(modeStr, "Mode: %2u", mode);
	lcd_string(0,7, modeStr, 0);
	
	sprintf(wheelTakeoffStr, "WheelTO: %4u/%4u", leftWheelTakeoff, rightWheelTakeoff);
	lcd_string(56, 1, wheelTakeoffStr, 0);
	sprintf(wheelLandingStr, "WheelLA: %4u/%4u", leftWheelLanding, rightWheelLanding);
	lcd_string(56, 2, wheelLandingStr, 0);
	
	sprintf(brakeLStr, "BrakeL: %5u", brakeL);
	lcd_string(56, 3, brakeLStr, 0);
	sprintf(brakeRStr, "BrakeR: %5u", brakeR);
	lcd_string(56, 4, brakeRStr, 0);
	
	createButtonLabels();
//	lcd_string(121, 2, buttonDebug, 0);
	lcd_string(121, 5, buttonSettings, 0);
	lcd_string(121, 7, buttonHome, 0);
}

void updateDebug()
{
	sprintf(takeOffStrUpd, "%4u", takeoff);
	lcd_string(36, 1, takeOffStrUpd, 0);
	sprintf(landingStrUpd, "%4u", landing);
	lcd_string(36, 2, landingStrUpd, 0);
	
	sprintf(wowLStrUpd, "%2u", wowL);
	lcd_string(24,3, wowLStrUpd, 0);
	sprintf(wowRStrUpd, "%2u", wowR);
	lcd_string(24,4, wowRStrUpd, 0);
	sprintf(wowCalStrUpd, "%2u", wowCal);
	lcd_string(24,5, wowCalStrUpd, 0);
	sprintf(IRStrUpd, "%2u", IR);
	lcd_string(16,6, IRStrUpd, 0);
	
	sprintf(modeStrUpd, "%2u", mode);
	lcd_string(24,7, modeStrUpd, 0);
	
	sprintf(wheelTakeoffStrUpd, "%4u/%4u", leftWheelTakeoff, rightWheelTakeoff);
	lcd_string(92, 1, wheelTakeoffStrUpd, 0);
	sprintf(wheelLandingStrUpd, "%4u/%4u", leftWheelLanding, rightWheelLanding);
	lcd_string(92, 2, wheelLandingStrUpd, 0);
	
	sprintf(brakeLStrUpd, "%5u", brakeL);
	lcd_string(88, 3, brakeLStrUpd, 0);
	sprintf(brakeRStrUpd, "%5u", brakeR);
	lcd_string(88, 4, brakeRStrUpd, 0);
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
