//-----------------------------------------------------------------------------
// File Name    : gcodeParser.cpp
// Description  : Converts a .GCode file into a custom format
// Author       : Liam Lawrence and Elijah Hodges
// Created      : April 30, 2018
//
// Version      : 1.0
// Last Updated : May 13, 2018
//-----------------------------------------------------------------------------

#include "gcodeParser.hpp"
#include "move.hpp"
#include "globals.h"
#include <thread>
#include <mutex>
#include <iostream>

#define ARC  1
#define LINE 0
#define CW   1
#define CCW  (-1)

std::mutex m1, m2, m3, m4;

// Parses laser intensity from the file
void readLaserIntensity(std::string command) {

    unsigned int start = command.find('S') + 1;
    unsigned int length = command.find(' ', start) - start;
    laserIntensity = std::stod(command.substr(start, length));
}


// TODO test this against a non-threaded version
// TODO test against a space-delimited version
// Parses X, Y, I, and J values from the file
void readXYIJ(std::string command, char variable) {
    unsigned int start = command.find(variable) + 1;
    unsigned int length = command.find(' ', start) - start;

    switch (variable) {
        case 'X':
            m1.lock();
            xCoord2 = SCALE * std::stod(command.substr(start, length));
            m1.unlock();
            return;

        case 'Y':
            m2.lock();
            yCoord2 = SCALE * std::stod(command.substr(start, length));
            m2.unlock();
            return;

        case 'I':
            m3.lock();
            iCoord = SCALE * std::stod(command.substr(start, length));
            m3.unlock();
            return;

        case 'J':
            m4.lock();
            jCoord = SCALE * std::stod(command.substr(start, length));
            m4.unlock();
            return;
    }
}


// Grabs multiple coordinates in parallel
void updateCoords(std::string &command, bool arc) {
    if (arc) {
        std::thread t1(readXYIJ, command, 'X');
        std::thread t2(readXYIJ, command, 'Y');
        std::thread t3(readXYIJ, command, 'I');
        std::thread t4(readXYIJ, command, 'J');
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    } else {
        std::thread t1(readXYIJ, command, 'X');
        std::thread t2(readXYIJ, command, 'Y');
        t1.join();
        t2.join();
    }
}


// Converts the .GCode file into a custom format
void convert() {
    std::string command;
    std::ofstream stepFile (STEPFILE_PATH);
    std::ifstream gCodeFile (GCODEFILE_PATH);
    char letter;

    // Loop to translate G-Code
    while(getline(gCodeFile, command)) {

        // Checks to see the Commands letter code
        letter = command.at(0);
        if (letter == 'G') {
//            updateCoords(command, 0);

            // Movement based commands
            switch(std::stoi(command.substr(1, 2))) {
//                case 0:
//                    updateCoords(command, LINE);
//                    movePoint(x2, y2);
//                    break;

                case 1:
                    updateCoords(command, LINE);
                    move(stepFile, LINE);
                    break;

                case 2:
                    updateCoords(command, ARC);
                    move(stepFile, CW);
                    break;

                case 3:
                    updateCoords(command, ARC);
                    move(stepFile, CCW);
                    break;
//
//                case 4:
//                    movePause();
//                    break;
            }

            // Update the current set of coordinates
            xCoord1 = xCoord2;
            yCoord1 = yCoord2;

        // Laser toggle commands
        } else if (letter == 'M') {
            switch (std::stoi(command.substr(1, 2))) {
                case 3:
                    laserEnabled = true;
                    stepFile << "0xA\n";
                    break;

                case 4:
                    // TODO - Write this living nightmare
                    break;

                case 5:
                    laserEnabled = false;
                    stepFile << "0xB\n";
            }

        // Laser intensity command
        } else if (command.at(0) == 'S') {
            readLaserIntensity(command);
            stepFile << "0xE\n";
            stepFile << "0x" << std::uppercase << std::hex << laserIntensity << "\n";
        }

    }

    stepFile.close();
    gCodeFile.close();
}

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

// Optimizes the step file
void clean() {
    std::string command;
    std::ifstream stepFile (STEPFILE_PATH);
    std::ofstream temp (TEMPFILE_PATH);
    int currentCommand;
    int lastCommand = 0;
    bool firstLoop = true;
    int optimizedCommand;

    // Main loop
    while(stepFile >> std::ws && getline(stepFile, command)) {

        // Set up the first loop
        if (firstLoop) {
            lastCommand = std::stoi(command, nullptr, 16);
            firstLoop = false;
            continue;
     }

        // Reads in the next command
        currentCommand = std::stoi(command, nullptr, 16);

        // If the last command is 0xE, skip optimizing the next two commands
        if (lastCommand == 0xE) {
            temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
            temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
            getline(stepFile, command);
            lastCommand = std::stoi(command, nullptr, 16);
            continue;
        }

        // If the current command is 0xE, skip optimizing the next three commands
        if (currentCommand == 0xE) {
            temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
            temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
            getline(stepFile, command);
            temp << command << "\n";
            getline(stepFile, command);
            lastCommand = std::stoi(command, nullptr, 16);
            continue;
        }

        // If the last command is a special case (other than 0xE) skip the next command
        if ((lastCommand & 0b1100) == 0b1000 || (lastCommand & 0b0011) == 0b0010) {
            temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
            lastCommand = currentCommand;
            continue;
        }

        // If the current comman is a special case (other than 0xE) skip the next two commands
        else if ((currentCommand & 0b1100) == 0b1000 || (currentCommand & 0b0011) == 0b0010) {
            temp << "0x" << std::hex << std::uppercase << lastCommand <<"\n";
            temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
            getline(stepFile, command);
            lastCommand = std::stoi(command, nullptr, 16);
            continue;
        } else {
             // Or the current and last commands together
             optimizedCommand = (lastCommand | currentCommand);

            // If the Or'd command can be optimized, write that and skip and move to a new set of two lines
            if ((optimizedCommand & 0b0101) == 0b0101) {
                 temp << "0x" << std::hex << std::uppercase << optimizedCommand << "\n";
                 getline(stepFile, command);
                 lastCommand = std::stoi(command, nullptr, 16);
                 continue;
             } else {
                 temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
                 lastCommand = currentCommand;
             }
        }
    }

    //TODO The last command is bugged and doesn't print correctly
    temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";


    // Closes the files and gets rid of the temporary file
    stepFile.close();
    temp.close();

    std::remove(STEPFILE_PATH.c_str());
    std::rename(TEMPFILE_PATH.c_str(), STEPFILE_PATH.c_str());
}