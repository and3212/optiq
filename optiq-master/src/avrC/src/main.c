//-----------------------------------------------------------------------------
// File Name    : main.c
// Description  : Main controller file for the TODO-NameHere
// Author       : Liam Lawrence
// Created      : April 16, 2018
//
// Version      : 1.0
// Compiler     : AVR-GCC 4.3.0; avr-libc 1.6.2 <-- TODO Edit this
// Hardware     : ATMega2560-XX
// Programmer   : BootLoader Blah blah <-- TODO Edit this
// Last Updated : n/a
//-----------------------------------------------------------------------------

// Macros for setting bits high and low
// #define SETBIT(ADDRESS, BIT) (ADDRESS |= (1 << BIT))
// #define CLEARBIT(ADDRESS, BIT) (ADDRESS &= ~(1 << BIT))
// PORTx &= ~(1 << PDn); // Pin PDn on PORTx goes low
// PORTx |= (1 << PDn);  // Pin PDN on PORTx goes high


// TODO LIST //
// SPI communication between ATMega(M) and Pi Zero(S)                     COMPLETED
// TODO - Stepper communication between ATMega and Stepper chips
// TODO - Limit switch safety and initialization
// TODO - Distance sensor safety
// TODO - Control over dual lasers

#include "../lib/SPI.h"
#include "../lib/sensors.h"
#include <avr/io.h>
#include <util/delay.h>

// 15 - SCLK
// 16 - MOSI
// 14 - MISO
// 10 - SS

// 9 & 8 - Stepper one
// 7 & 6 - Stepper two


// ========== PINOUT ============
// PIN -  NAME      1-OUT : 0-IN
// ==============================
// B0  -  SS              - 0
// B1  -  SCK             - 0
// B2  -  MOSI            - 1
// B3  -  MISO            - 1
// B4  -                  - 0
// B5  -                  - 1
// B6  -                  - 0
// B7  -                  - 0
// ==============================
// C0  -                  - 0
// C1  -                  - 1
// C2  -                  - 0
// C3  -                  - 1
// C4  -                  - 0
// C5  -                  - 1
// C6  -                  - ?
// ==============================
// D0  -  RX / DIV2       - 0
// D1  -  TX / Enable     - 0
// D2  -  FREQ/PHASE      - 0
// D3  -                  - ?
// D4  -  Keypad          - 0
// D5  -  Keypad          - 0
// D6  -  Keypad          - 0
// D7  -  Keypad          - 0
// ==============================

// RPi format
// int Gx   | double x| double y| bool laser
// Movement | X-coord | Y-coord | Laser (0 0 0 0 0 0 0 x)


int main() {
    bool run;
    uint8_t strength = 0;           // 0 is off, 1 is the safety laser, 2 is the real laser
    uint8_t movement;               // Gx - Where X is the value of movement
    double xCoord, yCoord;          // X and Y coordinate to move
    bool safeDistance = 0;          // Tells whether or not the laser cutter is near material
    double theta1, theta2;          // Angles that the arms should move
    uint32_t word;

    // Port Directions //
    DDRA = 0xff;
    DDRB = 0xff;
    DDRC = 0xff;
    DDRD = 0xff;

    initializeSPI();

    restart:
    // Initialize Variables //
    run = false;    //TODO replace this with a pin read

    // Wait for the "Start" command
    while(run == false) {
        // Read from RPi
        // receivedStart = PINx & (1<<pinNumb); //TODO change the thing in the while to just "PINx & (1<<pinNumb)"?
        _delay_ms(250);
        break;  //TODO remove
    }

    // Calibrate steppers using limit switches

    // Main loop
    for(;;) {
        // If for whatever reason the Pi says stop, stop
        if (run == false) {
            goto restart;
        }

        // Send
        word = SPI_Transceiver(0x4C);
        //TODO Check if these four are accurate
        // Currently these words are a byte each, subject to change
        movement = word >> 24;
        xCoord = 0xFF & (word >> 16);
        yCoord = 0xFF & (word >> 8);
        strength = (isOnTable()) ? 0xFF & word : 0;

        laserStrength(strength);
        moveServos(movement, xCoord, yCoord);
    }



    goto restart;

}
