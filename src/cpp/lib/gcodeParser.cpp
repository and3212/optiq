//
// Created by liam on 4/30/18.
//

#include "gcodeParser.hpp"
#include "move.hpp"
#include "globals.h"
#include <thread>
#include <mutex>
//#include <string>
#include <fstream>
#include <cstdio>
#include <iostream>

#define ARC  1
#define LINE 0
#define CW   1
#define CCW  (-1)

std::mutex m1, m2, m3, m4;
//double xCoord, yCoord, iCoord, jCoord;


void readLaserIntensity(std::string command) {

    unsigned int start = command.find('S') + 1;
    unsigned int length = command.find(' ', start) - start;
    laserIntensity = std::stod(command.substr(start, length));
}


// TODO test this against a non-threaded version
// TODO test against a space-delimited version
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


void convert() {
    std::string command;
    std::ofstream stepFile (STEPFILE_PATH);
    std::ifstream gCodeFile (GCODEFILE_PATH);

    // Loop to translate G-Code
    while(getline(gCodeFile, command)) {
        // Checks to see the Commands letter code

        if (command.at(0) == 'G') {
//            updateCoords(command, 0);

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

            xCoord1 = xCoord2;
            yCoord1 = yCoord2;

        } else if (command.at(0) == 'M') {
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

 void clean() {
     std::string command;
     std::ifstream stepFile (STEPFILE_PATH);
 //    std::ifstream stepFile ("../res/test.txt");
     std::ofstream temp (TEMPFILE_PATH);
     int currentCommand;
     int lastCommand = 0;
     bool firstLoop = true;
     int optimizedCommand;

     while(stepFile >> std::ws && getline(stepFile, command)) {
         if (firstLoop) {
             lastCommand = std::stoi(command, nullptr, 16);
             firstLoop = false;
             continue;
         }

         currentCommand = std::stoi(command, nullptr, 16);

         if (lastCommand == 0xE) {
             temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
             temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
             getline(stepFile, command);
             lastCommand = std::stoi(command, nullptr, 16);
             continue;
         }

         if (currentCommand == 0xE) {
             temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
             temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
             getline(stepFile, command);
             temp << command << "\n";
             getline(stepFile, command);
             lastCommand = std::stoi(command, nullptr, 16);
             continue;
         }

         // GOOD TO GO
         if ((lastCommand & 0b1100) == 0b1000 || (lastCommand & 0b0011) == 0b0010) {
             temp << "0x" << std::hex << std::uppercase << lastCommand << "\n";
             lastCommand = currentCommand;
             continue;
         }

         // Checks to see if current command contains any of our special cases
         else if ((currentCommand & 0b1100) == 0b1000 || (currentCommand & 0b0011) == 0b0010) {
             temp << "0x" << std::hex << std::uppercase << lastCommand <<"\n";
             temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";
             getline(stepFile, command);
             lastCommand = std::stoi(command, nullptr, 16);
             continue;
         } else {
             optimizedCommand = (lastCommand | currentCommand);

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

     temp << "0x" << std::hex << std::uppercase << currentCommand << "\n";


     stepFile.close();
     temp.close();

     std::remove(STEPFILE_PATH.c_str());
     std::rename(TEMPFILE_PATH.c_str(), STEPFILE_PATH.c_str());
 }