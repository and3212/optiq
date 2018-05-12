//
// Created by liam on 4/30/18.
//

#include "move.hpp"
#include "globals.h"
#include <cmath>
#include <iostream>

#define LINE 0
#define CW   1
#define CCW  (-1)

// It divides the line into segments
// It calculates the angle values at each of the (x, y) points
double findTheta(int angleNumber, int i, short movementType, double beta, double beta1, double R) {
    double stepX, stepY;

    if (movementType == LINE) {
        stepX = (xCoord1 + (i * (xCoord2 - xCoord1)) / SEGMENTS);
        stepY = (yCoord1 + (i * (yCoord2 - yCoord1)) / SEGMENTS);
    } else {
        stepX = iCoord + R * cos(beta1 + ((i * beta) / SEGMENTS));
        stepY = jCoord + R * sin(beta1 + ((i * beta) / SEGMENTS));
    }

    // TODO Make denominator a constant
    double angle2 = acos((stepX * stepX + stepY * stepY - (LENGTH1_SQUARED) - (LENGTH2_SQUARED)) / (2.0 * LENGTH1 * LENGTH2));

    if (angleNumber == 2)
        return angle2;

    // Determines if we need to add Pi to the result
    double addPi = (stepX < 0) ? M_PI : 0;

    // This returns angle1
    return addPi + atan(stepY / stepX) - atan((LENGTH2 * sin(angle2)) / (LENGTH1 + LENGTH2 * cos(angle2))) ;
}


void move(std::ofstream &stepFile, short movementType) {
    double R = 0;
    double beta = 0;
    double beta1 = 0;

    if (movementType != LINE) {
        R = sqrt((iCoord - xCoord1) * (iCoord - xCoord1) + (jCoord - yCoord1) * (jCoord - yCoord1));

        double addPi1 = (xCoord1 - iCoord > 0) ? 0 : M_PI;
        double addPi2 = (xCoord2 - iCoord > 0) ? 0 : M_PI;

        beta1 = addPi1 + atan((jCoord - yCoord1) / (iCoord - xCoord1));
        double beta2 = addPi2 + atan((jCoord - yCoord2) / (iCoord - xCoord2));
        beta = beta2 - beta1;

        if (movementType == CW) {
            if (beta > 0) {
                beta -= 2.0 * M_PI;
            }
        } else {
            if (beta < 0) {
                beta += 2.0 * M_PI;
            }
        }
    }

    // Sets up the initial theta values and remainders
    double theta1Initial = findTheta(1, 0, movementType, beta, beta1, R);
    double theta2Initial = findTheta(2, 0, movementType, beta, beta1, R);
    double deltaTheta1, deltaTheta2;
    double theta1Final, theta2Final;

    double numSteps1, numSteps2;
    int direction1, direction2;

    double slopeRemainder;
    double slopeStepCount;

    // Finds what the next (x, y) the thetas are for that point
    for (int i = 0; i < SEGMENTS; i++) {
        slopeRemainder = 0;
        theta1Final = findTheta(1, i+1, movementType, beta, beta1, R);
        theta2Final = findTheta(2, i+1, movementType, beta, beta1, R);

        deltaTheta1 = theta1Final - theta1Initial;
        deltaTheta2 = theta2Final - theta2Initial;


        // Determines the sign of the movement
        // TODO change this eventually to ANDing the sign bit 0xXX
        direction1 = (deltaTheta1 > 0) ? 1 : -1;
        direction2 = (deltaTheta2 > 0) ? 1 : -1;


        // Sets the number of steps in each "mini segment"
        //
        // Steps can only be integers, so store the decimals
        // as remainderX and add that onto the next step
        numSteps1 = (deltaTheta1 / MOTORSTEP) + remainder1;
        numSteps2 = (deltaTheta2 / MOTORSTEP) + remainder2;

        // Calculates the new remainder
        remainder1 = numSteps1 - (int)numSteps1;
        remainder2 = numSteps2 - (int)numSteps2;

        numSteps1 = std::abs((int)numSteps1);
        numSteps2 = std::abs((int)numSteps2);

        if (numSteps1 != 0) {
            for (int j = 0; j < numSteps1; j++) {
                angle1 += direction1 * MOTORSTEP;

                // Outputs to our step file
                if (direction1 > 0)
                    stepFile << "0x1\n";
                else
                    stepFile << "0x3\n";


                // Calculates the "slope" movements of the line
                slopeStepCount = (numSteps2 / numSteps1) + slopeRemainder;
                slopeRemainder = slopeStepCount - (int)slopeStepCount;
                slopeStepCount = (j != numSteps1-1) ? (int)slopeStepCount : std::round(slopeStepCount);

                // Moves the outer arm
                for (int k = 0; k < slopeStepCount; k++) {
                    angle2 += direction2 * MOTORSTEP;

                    // Outputs to our step file
                    if (direction2 > 0)
                        stepFile << "0x4\n";
                    else
                        stepFile << "0xC\n";
                }
            }
        } else {
            // Used if the inner arm doesn't have to move
            for (int k = 0; k < numSteps2; k++) {
                angle2 += direction2 * MOTORSTEP;

                // Outputs to our step file
                if (direction2 > 0)
                    stepFile << "0x4\n";
                else
                    stepFile << "0xC\n";
            }
        }

        theta1Initial = theta1Final;
        theta2Initial = theta2Final;
    }


}


//// TODO Test if I am passing references properly
//void moveLine(std::ofstream &stepFile) {
//    // Sets up the initial theta values and remainders
//    double theta1Initial = findThetaLine(1, 0);
//    double theta2Initial = findThetaLine(2, 0);
//    double deltaTheta1, deltaTheta2;
//    double theta1Final, theta2Final;
//
//    double numSteps1, numSteps2;
//    int direction1, direction2;
//
//    double slopeRemainder;
//    double slopeStepCount;
//
//    // Finds what the next (x, y) the thetas are for that point
//    for (int i = 0; i < SEGMENTS; i++) {
//        slopeRemainder = 0;
//        theta1Final = findThetaLine(1, i+1);
//        theta2Final = findThetaLine(2, i+1);
//
//        deltaTheta1 = theta1Final - theta1Initial;
//        deltaTheta2 = theta2Final - theta2Initial;
//
//
//        // Determines the sign of the movement
//        // TODO change this eventually to ANDing the sign bit 0xXX
//        direction1 = (deltaTheta1 > 0) ? 1 : -1;
//        direction2 = (deltaTheta2 > 0) ? 1 : -1;
//
//
//        // Sets the number of steps in each "mini segment"
//        //
//        // Steps can only be integers, so store the decimals
//        // as remainderX and add that onto the next step
//        numSteps1 = (deltaTheta1 / MOTORSTEP) + remainder1;
//        numSteps2 = (deltaTheta2 / MOTORSTEP) + remainder2;
//
//        // Calculates the new remainder
//        remainder1 = numSteps1 - (int)numSteps1;
//        remainder2 = numSteps2 - (int)numSteps2;
//
//        numSteps1 = std::abs((int)numSteps1);
//        numSteps2 = std::abs((int)numSteps2);
//
//        if (numSteps1 != 0) {
//            for (int j = 0; j < numSteps1; j++) {
//                angle1 += direction1 * MOTORSTEP;
//
//                // Outputs to our step file
//                if (direction1 > 0)
//                    stepFile << "0x1\n";
//                else
//                    stepFile << "0x3\n";
//
//
//                // Calculates the "slope" movements of the line
//                slopeStepCount = (numSteps2 / numSteps1) + slopeRemainder;
//                slopeRemainder = slopeStepCount - (int)slopeStepCount;
//                slopeStepCount = (j != numSteps1-1) ? (int)slopeStepCount : std::round(slopeStepCount);
//
//                // Moves the outer arm
//                for (int k = 0; k < slopeStepCount; k++) {
//                    angle2 += direction2 * MOTORSTEP;
//
//                    // Outputs to our step file
//                    if (direction2 > 0)
//                        stepFile << "0x4\n";
//                    else
//                        stepFile << "0xC\n";
//                }
//            }
//        } else {
//            // Used if the inner arm doesn't have to move
//            for (int k = 0; k < numSteps2; k++) {
//                angle2 += direction2 * MOTORSTEP;
//
//                // Outputs to our step file
//                if (direction2 > 0)
//                    stepFile << "0x4\n";
//                else
//                    stepFile << "0xC\n";
//            }
//        }
//
//        theta1Initial = theta1Final;
//        theta2Initial = theta2Final;
//    }
//}