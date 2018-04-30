//
// Created by liam on 4/30/18.
//

#ifndef CONVERTER_MOVE_HPP
#define CONVERTER_MOVE_HPP

void findThetaLine(bool lineOne, unsigned short NAMECHANGE);

void movePoint(double xCoord, double yCoord);

void moveLine(double xCoord, double yCoord);

void moveArc(double xCoord, double yCoord, double iCoord, double jCoord, bool clockwise);

void movePause();


#endif //CONVERTER_MOVE_HPP
