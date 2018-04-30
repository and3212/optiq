//
// Created by liam on 4/30/18.
//

#include "gcodeParser.hpp"
#include "move.hpp"
#include <thread>
#include <mutex>
//#include <string>
#include <fstream>

#define ARC  true
#define LINE false
#define CW   true
#define CCW  false

std::mutex m1, m2, m3, m4;
double xCoord, yCoord, iCoord, jCoord;

// TODO test this against a non-threaded version
// TODO test against a space-delimited version
void readXYIJ(std::string command, char variable) {
    unsigned int start = command.find(variable) + 1;
    unsigned int length = command.find(' ', start) - start;

    switch (variable) {
        case 'X':
            m1.lock();
            xCoord = std::stod(command.substr(start, length));
            m1.unlock();
            return;

        case 'Y':
            m2.lock();
            yCoord = std::stod(command.substr(start, length));
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
    std::ofstream stepFile ("../res/step.txt");
    std::ifstream gCodeFile ("../res/design.gcode");

    // Loop to translate G-Code
    while(getline(gCodeFile, command)) {
        // Checks to see the Commands letter code

        if (command.at(0) == 'G') {
            updateCoords(command, 0);

            switch(std::stoi(command.substr(1, 2))) {
                case 0:
                    updateCoords(command, LINE);
                    movePoint(xCoord, yCoord);
                    break;

                case 1:
                    updateCoords(command, LINE);
                    moveLine(xCoord, yCoord);
                    break;

                case 2:
                    updateCoords(command, ARC);
                    moveArc(xCoord, yCoord, iCoord, jCoord, CW);
                    break;

                case 3:
                    updateCoords(command, ARC);
                    moveArc(xCoord, yCoord, iCoord, jCoord, CCW);
                    break;

                case 4:
                    movePause();
                    break;
            }
        }

    }

    stepFile.close();
    gCodeFile.close();
}