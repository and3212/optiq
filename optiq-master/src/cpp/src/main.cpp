//-----------------------------------------------------------------------------
// File Name    : main.cpp
// Description  : Main file for the Optiq control code
// Author       : Liam Lawrence
// Created      : April 27, 2018
//
// Version      : 1.0
// Last Updated : May 13, 2018
//-----------------------------------------------------------------------------


#include <cmath>
#include <iostream>
#include "../lib/globals.h"
#include "../lib/run.hpp"
#include "../lib/gcodeParser.hpp"

// Global variable initialization
//TODO I think globals are bad, make them non-global?
unsigned const int SEGMENTS = 60;
const double PICTURE_SCALE = 1.0;
const double METRIC = 25.4;
const double TRANSLATE_X = 0;
const double TRANSLATE_Y = 0;
double remainder1 = 0.0;
double remainder2 = 0.0;
double angle1 = 0;
double angle2 = 0;
double xCoord1 =12;
double yCoord1 = 0;
double xCoord2 = 0;
double yCoord2 = 0;
double iCoord = 0;
double jCoord = 0;
double speed = 1.5; //in/second * (SCALE units/1 in)
double totalDistance = 0;
double scale = PICTURE_SCALE/METRIC;
int moveType = 0;
int laserIntensity = 0;
bool laserEnabled = false;
bool laserState = false;
const double LENGTH1 = 6;
const double LENGTH2 = 6;
const double LENGTH1_SQUARED = LENGTH1 * LENGTH1;
const double LENGTH2_SQUARED = LENGTH2 * LENGTH2;
const double MOTORSTEP = (0.05) * M_PI / 180.0;

const std::string STEPFILE_PATH = "C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\step.txt";
const std::string GCODEFILE_PATH = "C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\design.gcode";
const std::string TEMPFILE_PATH = "C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\temp.txt";
const std::string TIMEFILE_PATH = "C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\time.txt";
const std::string SPEEDFILE_PATH = "C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\speed.txt";

const int PROCESSING_SPEED = 10;

int main() {

    // Converts the .gcode file to a custom formatted "step file"
    convert();
    //system("../../scripts/removeNewLine.sh");

    // optimizes the gcode
    //clean();

    // Runs the processing simulation
    //system(("../../scripts/simulate.sh " + std::to_string(MOTORSTEP) + " " + std::to_string(PROCESSING_SPEED)).c_str());
    std::cout << totalDistance;
    return 0;
}
