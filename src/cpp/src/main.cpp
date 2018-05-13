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
#include "../lib/globals.h"
#include "../lib/run.hpp"
#include "../lib/gcodeParser.hpp"

// Global variable initialization
//TODO I think globals are bad, make them non-global?
unsigned const int SEGMENTS = 60;
const double SCALE = 40.0;
double remainder1 = 0;
double remainder2 = 0;
double angle1 = 0;
double angle2 = 0;
double xCoord1 =12 * SCALE;
double yCoord1 = 0 * SCALE;
double xCoord2 = 0 * SCALE;
double yCoord2 = 0 * SCALE;
double iCoord = 0 * SCALE;
double jCoord = 0 * SCALE;
int laserIntensity = 0;
bool laserEnabled = false;
unsigned const int LENGTH1 = 6 * SCALE;
unsigned const int LENGTH2 = 6 * SCALE;
unsigned const int LENGTH1_SQUARED = LENGTH1 * LENGTH1;
unsigned const int LENGTH2_SQUARED = LENGTH2 * LENGTH2;
const double MOTORSTEP = (0.05) * M_PI / 180.0;

const std::string STEPFILE_PATH = "../res/step.txt";
const std::string GCODEFILE_PATH = "../res/design.gcode";
const std::string TEMPFILE_PATH = "../res/temp.txt";

const int PROCESSING_SPEED = 10;

int main() {

    // Converts the .gcode file to a custom formatted "step file"
    convert();
    system("../../scripts/removeNewLine.sh");

    // optimizes the gcode
    //clean();

    // Runs the processing simulation
    system(("../../scripts/process.sh " + std::to_string(MOTORSTEP) + " " + std::to_string(PROCESSING_SPEED)).c_str());

    return 0;
}
