#ifndef BRAKES_H
#define BRAKES_H

void initBrakes(void);
void setBrakes();

#define steer PORTFbits.RF2
#define magnitude PORTFbits.RF3

#endif
