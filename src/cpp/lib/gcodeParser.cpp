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

#define ARC  1
#define LINE 0
#define CW   1
#define CCW  (-1)

std::mutex m1, m2, m3, m4;
//double xCoord, yCoord, iCoord, jCoord;

// TODO test this against a non-threaded version
// TODO test against a space-delimited version
void readXYIJ(std::string command, char variable) {
    unsigned int start = command.find(variable) + 1;
    unsigned int length = command.find(' ', start) - start;

    switch (variable) {
        case 'X':
            m1.lock();
            xCoord2 = std::stod(command.substr(start, length));
            m1.unlock();
            return;

        case 'Y':
            m2.lock();
            yCoord2 = std::stod(command.substr(start, length));
            m2.unlock();
            return;

        case 'I':
            m3.lock();
            iCoord = std::stod(command.substr(start, length));
            m3.unlock();
            return;

        case 'J':
            m4.lock();
            jCoord = std::stod(command.substr(start, length));
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
        }

    }

    stepFile.close();
    gCodeFile.close();
}

void clean() {
    std::string command;
    std::ifstream stepFile (STEPFILE_PATH);
    std::ofstream temp (TEMPFILE_PATH);
    int currentCommand;
    int lastCommand = 0;

    bool start = false;
    bool skip = false;
    int optimizedCommand;

    while(getline(stepFile, command)) {
        if (!start) {
            lastCommand = std::stoi(command, nullptr, 16);
            start = true;
            continue;
        }

        currentCommand = std::stoi(command, nullptr, 16);

        // Moves down a line with the input file
        if (skip) {
            skip = false;
            lastCommand = currentCommand;
            continue;
        }

        if ((lastCommand | currentCommand) & 0x5 == 0x5) {
            optimizedCommand = lastCommand | currentCommand;
            temp << std::hex << std::uppercase << "0x" << optimizedCommand << "\n";
            skip = true;

        } else {
            temp << std::hex << std::uppercase << "0x" << lastCommand << "\n";
        }

        lastCommand = currentCommand;

    }

    stepFile.close();
    temp.close();

//    std::remove(STEPFILE_PATH.c_str());
//    std::rename(TEMPFILE_PATH.c_str(), STEPFILE_PATH.c_str());
}