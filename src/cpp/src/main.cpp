#include "../lib/globals.h"
#include "../lib/run.hpp"
#include "../lib/gcodeParser.hpp"

// Global variables declarations
float length1 = 6;
float length2 = 6;
float motorStep = 0.5;
int segments = 60;

int main() {

    // Converts the .gcode file to a custom formatted "step file"
    convert();

    //run();


    return 0;
}
