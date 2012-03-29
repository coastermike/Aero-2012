#ifndef WHEELS_H
#define WHEELS_H

void initWheels(void);
void calibrateWOW();

#define leftHallBottom PORTDbits.RD8
#define leftHallTop PORTDbits.RD9
#define rightHallTop PORTDbits.D15
#define rightHallBottom PORTFbits.RF4

#endif
