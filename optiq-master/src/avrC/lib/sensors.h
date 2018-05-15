
#ifndef AVRC_SENSORS_H
#define AVRC_SENSORS_H

#include <avr/io.h>

// Sets the strength of the laser
void laserStrength(uint8_t strength);

// Checks to see if the arm is on the isOnTable
bool isOnTable();

#endif //AVRC_SENSORS_H
