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
#include <iostream>

#define RAPID 3
#define LINE 0
#define CW   1
#define CCW  -1
#define PAUSE 4

void setValue(std::string valueString, std::ofstream &stepFile, std::ofstream &timeFile) {
    char letter;

    letter = valueString.at(0);
    switch (letter) {
        case 'G':
            // Movement based commands
            switch (std::stoi(valueString.substr(1, valueString.length()))) {
                case 0:
                    moveType = RAPID;
                    laserState = laserEnabled;
                    laserEnabled = false;
                    stepFile << "0xB\n";
                    timeFile << "10\n";
                    break;

                case 1:
                    moveType = LINE;
                    break;

                case 2:
                    moveType = CW;
                    break;

                case 3:
                    moveType = CCW;
                    break;

                case 4:
                    moveType = PAUSE;
                    break;

                case 20:
                    moveType = PAUSE;
                    scale = PICTURE_SCALE;
                    break;

                case 21:
                    moveType = PAUSE;
                    scale = PICTURE_SCALE/METRIC;
            }
            break;

        case 'X':
            xCoord2 = (scale * std::stod(valueString.substr(1, valueString.length()))) + TRANSLATE_X;
            break;

        case 'Y':
            yCoord2 = (scale * std::stod(valueString.substr(1, valueString.length()))) + TRANSLATE_Y;
            break;

        case 'I':
            iCoord = (scale * std::stod(valueString.substr(1, valueString.length()))) + TRANSLATE_X;
            break;

        case 'J':
            jCoord = (scale * std::stod(valueString.substr(1, valueString.length()))) + TRANSLATE_Y;
            break;

        case 'R':
            moveType = LINE;
            break;

        case 'M':

            switch (std::stoi(valueString.substr(1, valueString.length()))) {
                case 3:
                    laserEnabled = true;
                    stepFile << "0xA\n";
                    timeFile << "10\n";
                    break;

                case 4:
                    // TODO - Write this living nightmare
                    break;

                case 5:
                    laserEnabled = false;
                    stepFile << "0xB\n";
                    timeFile << "10\n";
                    break;

                case 106:
                    laserEnabled = true;
                    stepFile << "0xA\n";
                    timeFile << "10\n";
                    break;

                case 107:
                    laserEnabled = false;
                    stepFile << "0xB\n";
                    timeFile << "10\n";
                    break;
            }
            moveType = PAUSE;
            break;

        case 'S':
            stepFile << "0xE\n";
            timeFile << "10\n";
            stepFile << "0x" << std::uppercase << std::hex << std::stoi(valueString.substr(1, valueString.length()))
                     << "\n";
            timeFile << "10\n";
            moveType = PAUSE;
            break;

        case 'P':
            stepFile << "0x0\n";
            timeFile << (int)std::stod(valueString.substr(1, valueString.length())) << "\n";
            break;

        case 'F':
            stepFile << "0x0\n";
            speed = (scale/PICTURE_SCALE) * (std::stod(valueString.substr(1, valueString.length()))/60);
            timeFile << "10\n";
            break;

        default:
            std::cout << valueString << "\n";
            break;
    }
}

// Converts the .GCode file into a custom format
    void convert() {
        std::string command;
        std::string newPiece;
        std::ofstream stepFile(STEPFILE_PATH);
        std::ofstream timeFile(TIMEFILE_PATH);
        std::ofstream speedFile(SPEEDFILE_PATH);
        std::ifstream gCodeFile(GCODEFILE_PATH);
        int index;

        // Loop to translate G-Code
        while (getline(gCodeFile, command)) {
            speedFile<<speed<<"\n";

            if (command.find(";") != std::string::npos){
                command = command.substr(0, command.find(";"));
            }

            if (command.find(" ") == std::string::npos && (command.find(" ") != command.length()-1)) {
                setValue(command, stepFile, timeFile);
            }

            while (command.find(" ") != std::string::npos) {
                index = command.find(" ");
                newPiece = command.substr(0, index);
                setValue(newPiece, stepFile, timeFile);
                command = command.substr(index + 1, command.length() - (index + 1));
                if (command.find(" ") == std::string::npos && (command.find(" ") != command.length()-1)) {
                    setValue(command, stepFile, timeFile);
                }
            }

            switch (moveType) {
                case RAPID:
                    move1(stepFile, timeFile, RAPID);
                    break;

                case LINE:
                    move1(stepFile, timeFile, LINE);
                    break;

                case CW:
                    move1(stepFile, timeFile, CW);
                    break;

                case CCW:
                    move1(stepFile, timeFile, CCW);
                    break;

                case PAUSE:
                    //Nothing, you're paused
                    break;
            }

            if(moveType == RAPID) {
                laserEnabled = laserState;
                if(laserEnabled){
                    stepFile << "0xA\n";
                    timeFile << "10\n";
                }
            }

            xCoord1 = xCoord2;
            yCoord1 = yCoord2;
        }

        stepFile.close();
        gCodeFile.close();
        timeFile.close();
    }

//// 0000 - 0x0 - Stop all motors
//
//// 0001 - 0x1 - Move I forwards
//// 0011 - 0x3 - Move I backwards
//// 0100 - 0x4 - Move O forwards
//// 1100 - 0xC - Move O backwards
//
//// 0101 - 0x5 - Move I and O forwards
//// 0111 - 0x7 - Move I backwards and O forwards
//// 1101 - 0xD - Move I forwards and O backwards
//// 1111 - 0xF - Move I and O backwards
//
//// Special cases
//// 1010 - 0xA - Turn laser on
//// 1011 - 0xB - Turn laser off
//// 1110 - 0xE - Set laser intensity to the next byte
//// 0010 - 0x2 - Set speed to the next byte
//
//// Optimizes the step file
//
//    void clean() {
//    std::string command;
//    std::ifstream stepFile (STEPFILE_PATH);
//    std::ofstream temp (TEMPFILE_PATH);
//    int currentCommand;
//    int lastCommand = 0;
//    bool firstLoop = true;
//    int optimizedCommand;
//
//    // Main loop
//    while(stepFile >> std::ws && getline(stepFile, command)) {
//
//        // Set up the first loop
//        if (firstLoop) {
//            lastCommand = std::stoi(command, nullptr, 16);
//            firstLoop = false;
//            continue;
//     }
//
//        // Reads in the next command
//        currentCommand = std::stoi(command, nullptr, 16);
//
//        // If the last command is 0xE, skip optimizing the next two commands
//        if (lastCommand == 0xE) {
//            temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
//            temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
//            getline(stepFile, command);
//            lastCommand = std::stoi(command, nullptr, 16);
//            continue;
//        }
//
//        // If the current command is 0xE, skip optimizing the next three commands
//        if (currentCommand == 0xE) {
//            temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
//            temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
//            getline(stepFile, command);
//            temp << command << "\n";
//            getline(stepFile, command);
//            lastCommand = std::stoi(command, nullptr, 16);
//            continue;
//        }
//
//        // If the last command is a special case (other than 0xE) skip the next command
//        if ((lastCommand & 0b1100) == 0b1000 || (lastCommand & 0b0011) == 0b0010) {
//            temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
//            lastCommand = currentCommand;
//            continue;
//        }
//
//        // If the current comman is a special case (other than 0xE) skip the next two commands
//        else if ((currentCommand & 0b1100) == 0b1000 || (currentCommand & 0b0011) == 0b0010) {
//            temp << "0x" << std::hex << std::uppercase << lastCommand <<"\n";
//            temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
//            getline(stepFile, command);
//            lastCommand = std::stoi(command, nullptr, 16);
//            continue;
//        } else {
//             // Or the current and last commands together
//             optimizedCommand = (lastCommand | currentCommand);
//
//            // If the Or'd command can be optimized, write that and skip and move to a new set of two lines
//            if ((optimizedCommand & 0b0101) == 0b0101) {
//                 temp << "0x" << std::hex << std::uppercase << optimizedCommand << "\n";
//                 getline(stepFile, command);
//                 lastCommand = std::stoi(command, nullptr, 16);
//                 continue;
//             } else {
//                 temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
//                 lastCommand = currentCommand;
//             }
//        }
//    }
//
//    //TODO The last command is bugged and doesn't print correctly
//    temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
//
//
//    // Closes the files and gets rid of the temporary file
//    stepFile.close();
//    temp.close();
//
//    std::remove(STEPFILE_PATH.c_str());
//    std::rename(TEMPFILE_PATH.c_str(), STEPFILE_PATH.c_str());
//}