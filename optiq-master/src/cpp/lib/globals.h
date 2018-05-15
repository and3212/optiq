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
extern double speed;
extern double totalDistance;
extern double scale;
extern int moveType;
extern int laserIntensity;
extern bool laserEnabled;
extern bool laserState; //tempVariable for G0


extern const double PICTURE_SCALE;
extern const double METRIC;
extern const double TRANSLATE_X;
extern const double TRANSLATE_Y;

extern const double LENGTH1;
extern const double LENGTH2;
extern const double LENGTH1_SQUARED;
extern const double LENGTH2_SQUARED;

extern const std::string STEPFILE_PATH;
extern const std::string GCODEFILE_PATH;
extern const std::string TEMPFILE_PATH;
extern const std::string TIMEFILE_PATH;
extern const std::string SPEEDFILE_PATH;





#endif //CPP_GLOBALVARIABLES_H
