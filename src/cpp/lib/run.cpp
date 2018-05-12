//
// Created by liam on 4/27/18.
//

#include <fstream>
#include "run.hpp"

// outerDir - outerEnable - innerDir - innerEnable
// 0000 - 0x0 - Stop all motors

// 0001 - 0x1 - Move I forwards
// 0011 - 0x3 - Move I backwards
// 0100 - 0x4 - Move O forwards
// 1100 - 0xC - Move O backwards

// 0101 - 0x5 - Move I and O forwards
// 0111 - 0x7 - Move I backwards and O forwards
// 1101 - 0xD - Move I forwards and O backwards
// 1111 - 0xF - Move I and O backwards

// Special cases
// 1010 - 0xA - Turn laser on
// 1011 - 0xB - Turn laser off
// 1110 - 0xE - Set laser intensity to the next byte
// 0010 - 0x2 - Set speed to the next byte


void run() {
    std::string command;
    std::ifstream steps ("../res/step.txt");

    // Initialize SPI communication
    // Set "RUN" pin to high

    while(getline(steps, command)) {
        switch(std::stoi(command)) {
            case 0x0:

                break;

            case 0x1:

                break;

            case 0x2:

                break;

            case 0x3:

                break;
        }
    }

    // Set "RUN" pin to low
    // Disable SPI

    steps.close();
}