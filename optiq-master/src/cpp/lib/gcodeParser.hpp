//
// Created by liam on 4/30/18.
//

#ifndef CONVERTER_GCODEALGORITHM_HPP
#define CONVERTER_GCODEALGORITHM_HPP

#include <string>

// Sets global variables from G-code
void setValue(std::string valueString, std::ofstream &stepFile, std::ofstream &timeFile);

// Converts G-Code to custom step
void convert();

// Cleans up and optimizes the G-Code
void clean();

#endif //CONVERTER_GCODEALGORITHM_HPP
