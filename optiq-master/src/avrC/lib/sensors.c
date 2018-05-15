//-----------------------------------------------------------------------------
// File Name    : sensors.c
// Description  : Reads data from the limits switches and the distance sensor
// Author       : Liam Lawrence
// Created      : April 18, 2018
//
// Version      : 1.0
// Compiler     : AVR-GCC 4.3.0; avr-libc 1.6.2 <-- TODO Edit this
// Hardware     : ATMega2560-XX
// Programmer   : BootLoader Blah blah <-- TODO Edit this
// Last Updated : n/a
//-----------------------------------------------------------------------------


#include "sensors.h"
#include <avr/io.h>         //TODO Do I put this here or in sensors.h?


void laserStrength(uint8_t strength) {
    switch (strength) {
        case 0:
            // Turn off both lasers
            break;
        case 1:
            // Turn on the dummy laser and turn off the real laser
            break;
        case 2:
            // Turn off the dummy laser and turn on the real laser
            break;
    }
}

bool isOnTable() {
    return true;
}
