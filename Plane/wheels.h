#ifndef WHEELS_H
#define WHEELS_H

void initWheels(void);
void calibrateWOW();

#define leftHallTop PORTDbits.RD8
#define leftHallBottom PORTDbits.RD9
#define rightHallTop PORTDbits.D15
#define rightHallBottom PORTFbits.RF4

#endif
