//
// Created by liam on 4/27/18.
//

#include <fstream>
#include "run.hpp"

// yDir - yEnable - xDir - xEnable
// 0000 - 0x0 - Stop all motors
// 0001 - 0x1 - Move X forwards
// 0011 - 0x3 - Move X backwards
// 0100 - 0x4 - Move Y forwards
// 1100 - 0xC - Move Y backwards
// 0101 - 0x5 - Move X and Y forwards
// 0111 - 0x7 - Move X forwards and Y backwards
// 1101 - 0xD - Move X backwards and Y forwards
// 1111 - 0xF - Move X and Y backwards

// 1010 - 0xA - Set laser intensity to next byte
// 1011 - 0xB - Set speed to next byte


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