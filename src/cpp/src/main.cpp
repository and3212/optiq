#include <math.h>
#include "../lib/globals.h"
#include "../lib/run.hpp"
#include "../lib/gcodeParser.hpp"
#include <string>

// Global variable initialization
unsigned const int SEGMENTS = 60;
double remainder1 = 0;
double remainder2 = 0;
double angle1 = 0;
double angle2 = 0;
double xCoord1 = 12;
double yCoord1 = 0;
double xCoord2 = 0;
double yCoord2 = 0;
double iCoord = 0;
double jCoord = 0;
unsigned const int LENGTH1 = 240;
unsigned const int LENGTH2 = 240;
unsigned const int LENGTH1_SQUARED = LENGTH1 * LENGTH1;
unsigned const int LENGTH2_SQUARED = LENGTH2 * LENGTH2;
const double MOTORSTEP = (0.5) * M_PI / 180.0;    //TODO CHANGE ME IDIOTS

const std::string STEPFILE_PATH = "../res/step.txt";
const std::string GCODEFILE_PATH = "../res/design.gcode";
const std::string TEMPFILE_PATH = "../res/temp.txt";

int main() {

    // Converts the .gcode file to a custom formatted "step file"
    convert();

    //clean();

    //run();


    return 0;
}
