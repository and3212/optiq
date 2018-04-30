//-----------------------------------------------------------------------------
// File Name    : spi.c
// Description  : SPI communication and protocol
// Author       : Liam Lawrence
// Created      : April 16, 2018
//
// Version      : 1.0
// Compiler     : AVR-GCC 4.3.0; avr-libc 1.6.2 <-- TODO Edit this
// Hardware     : ATMega2560-XX
// Programmer   : BootLoader Blah blah <-- TODO Edit this
// Last Updated : April 18, 2018
//-----------------------------------------------------------------------------

#include "SPI.h"
#include <avr/io.h>         //TODO Do I put this here or in SPI.h?

#define DDR_SPI DDRB
//#define SS    0
#define SCK     1
#define MOSI    2
//#define MISO  3


void initializeSPI(void) {
    // Set MOSI and SCK as outputs, all others are inputs
    DDR_SPI = (1 << MOSI)|(1 << SCK);

    //Enable SPI, Master, set clock rate fck/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

unsigned char SPI_Transceiver (unsigned char data) {
    //TODO Or should I change the comments so they are stacked on top of each line?
    SPDR = data;                    // Load data into the buffer
    while(!(SPSR & (1 << SPIF)));   // Wait until transmission complete
    return(SPDR);                   // Return received data
}
