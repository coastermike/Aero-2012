#ifndef WHEELS_H
#define WHEELS_H

void initWheels(void);
void calibrateWOW();
void calGround();
void calAir();
void reset();
void start();

#define leftHallBottom PORTDbits.RD8
#define leftHallTop PORTDbits.RD9
#define rightHallTop PORTFbits.RF4
#define rightHallBottom PORTDbits.RD15

#endif
