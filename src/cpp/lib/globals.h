//
// Created by liam on 4/26/18.
//

#ifndef CPP_GLOBALVARIABLES_H
#define CPP_GLOBALVARIABLES_H

#include <string>

extern unsigned const int SEGMENTS;
extern double remainder1;
extern double remainder2;
extern const double MOTORSTEP;     // The physical minimum that the stepper motors can move

extern double angle1;
extern double angle2;

extern double xCoord1;
extern double yCoord1;
extern double xCoord2;
extern double yCoord2;
extern double iCoord;
extern double jCoord;

extern unsigned const int LENGTH1;
extern unsigned const int LENGTH2;
extern unsigned const int LENGTH1_SQUARED;
extern unsigned const int LENGTH2_SQUARED;

extern const std::string STEPFILE_PATH;
extern const std::string GCODEFILE_PATH;
extern const std::string TEMPFILE_PATH;





#endif //CPP_GLOBALVARIABLES_H
