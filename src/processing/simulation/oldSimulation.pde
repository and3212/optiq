// // Length of the arms
// int l1 = 240;
// int l2 = 240;
//
// // Angles of the arms
// float theta1 = 0.0;
// float theta2 = 0.0;
// int x1;
// int y1;
// int x2;
// int y2;
// float MOTORSTEP = (0.05*PI/180.0);
// float addPi1;
// float addPi2;
//
// // Points for the lines to display the arms
// int px1 = 0;
// int py1 = 0;
// int px2;
// int py2;
// int px3;
// int py3;
//
// String[] command;
// boolean laserEnabled = false;
// int laserIntensity = 255;
//
// IntList xCoord_ON;
// IntList yCoord_ON;
// IntList laserStrength_PerPoint;
// IntList xCoord_OFF;
// IntList yCoord_OFF;
//
// void setup() {
//     size(1080, 1000);
//     background(0);
//     frameRate(120);
//     xCoord_ON = new IntList();
//     yCoord_ON = new IntList();
//     xCoord_OFF = new IntList();
//     yCoord_OFF = new IntList();
//     laserStrength_PerPoint = new IntList();
//     command = loadStrings("step.txt");
// }
//
// // Loop values
// int i = 0;
//
// // Main loop
// void draw() {
//     background(0);
//
//     // Shifts the origin to the bottom left
//     scale(1, -1);
//     translate(60, (60 + (l1+l2)/2) -height);
//
//     // Draws an arc that represents the max cutting size
//     noFill();
//     stroke(255);
//     strokeWeight(3);
//     arc(l1 + l2, 0, 2 * (l1 + l2), 2 * (l1 +  l2), 0, PI);
//     translate(l1 + l2, 0);  // Move the origin to the center of the arc
//
//     // Draws the end of the "Table"
//     strokeWeight(3);
//     line(-width, 0, width, 0);
//
// /////////////////////////////////////////////////////////////////////
//
//     //Math for angles
//     switch (command[i]) {
//         case "0x1":
//             theta1 += MOTORSTEP;
//             break;
//         case "0x3":
//             theta1 -= MOTORSTEP;
//             break;
//         case "0x4":
//             theta2 += MOTORSTEP;
//             break;
//         case "0xC":
//             theta2 -= MOTORSTEP;
//             break;
//
//         case "0x5":
//             theta1 += MOTORSTEP;
//             theta2 += MOTORSTEP;
//             break;
//         case "0x7":
//             theta1 -= MOTORSTEP;
//             theta2 += MOTORSTEP;
//         case "0xD":
//             theta1 += MOTORSTEP;
//             theta2 -= MOTORSTEP;
//         case "0xF":
//             theta1 -= MOTORSTEP;
//             theta2 -= MOTORSTEP;
//
//         case "0xA":
//             laserEnabled = true;
//             break;
//         case "0xB":
//             laserEnabled = false;
//             break;
//         case "0xE":
//             i++;
//             if (i > command.length - 1) {
//                 exit();
//             }
//             laserIntensity = unhex(command[i].substring(2, command[i].length()));
//     }
//
//
//     // println(theta1 + addPi1);
//     px2 = (int)(cos(theta1) * l1);
//     py2 = (int)(sin(theta1) * l1);
//     // println(theta2+addPi2);
//     px3 = (int)(cos(theta2 + theta1) * l2) + px2;
//     py3 = (int)(sin(theta2 + theta1) * l2) + py2;
//
// ///////////////////////////////////////////////////////////
//     // Draws the arms
//     strokeWeight(15);
//     stroke(0, 50, 100);
//     line(px1, py1, px2, py2);
//     stroke(100, 0, 50);
//     line(px2, py2, px3, py3);
//     stroke(58, 4, 81);
//     point(px2, py2);
//
//     if (laserEnabled) {
//         xCoord_ON.append(px3);
//         yCoord_ON.append(py3);
//         laserStrength_PerPoint.append(laserIntensity);
//     } else {
//         xCoord_OFF.append(px3);
//         yCoord_OFF.append(py3);
//     }
//
//     strokeWeight(2);
//     for (int j = 0; j < xCoord_ON.size(); j++) {
//         stroke(0, 100, 50, laserStrength_PerPoint.get(j));
//         point(xCoord_ON.get(j), yCoord_ON.get(j));
//     }
//
//     for (int j = 0; j < xCoord_OFF.size(); j++) {
//         stroke(100, 50, 0, 100);
//         point(xCoord_OFF.get(j), yCoord_OFF.get(j));
//     }
//
//
//     i++;
//     if (i > command.length-1) {
//         delay(5000);
//         exit();
//     }
// }
