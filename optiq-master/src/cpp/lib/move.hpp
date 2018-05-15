//
// Created by liam on 4/30/18.
//

#ifndef CONVERTER_MOVE_HPP
#define CONVERTER_MOVE_HPP

#include <fstream>

double findTheta(int angleNumber, int i, bool isLine, double beta, double beta1, double R);

void move1(std::ofstream &stepFile, std::ofstream &timeFile, short movementType);

//void movePoint(double xCoord, double yCoord);
//
//void moveLine(std::ofstream &stepFile);
//
//void moveArc(std::ofstream &stepFile, bool clockwise);
//
//void movePause();


#endif //CONVERTER_MOVE_HPP
