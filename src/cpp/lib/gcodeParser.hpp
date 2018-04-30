//
// Created by liam on 4/30/18.
//

#ifndef CONVERTER_GCODEALGORITHM_HPP
#define CONVERTER_GCODEALGORITHM_HPP

#include <string>

// Updates the global X and Y variables by reading the current G-Code line
void readXYIJ(std::string command, char variable);

// Runs findXY simultaneously for X and Y
void updateCoords(std::string &command, bool arc);

// Converts G-Code to custom step
void convert();

#endif //CONVERTER_GCODEALGORITHM_HPP
