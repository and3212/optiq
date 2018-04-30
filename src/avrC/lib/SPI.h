#ifndef AVRC_SPI_H
#define AVRC_SPI_H


// Initialize SPI communication with the AVR as the master
void initializeSPI(void);

// Send and receive data
unsigned char SPI_Transceiver (unsigned char data);


#endif //AVRC_SPI_H
