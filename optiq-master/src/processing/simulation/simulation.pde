//-----------------------------------------------------------------------------
// File Name    : simulation.pde
// Description  : Processing simulation for the Optiq laser cutter
// Author       : Liam Lawrence
// Created      : May 13, 2018
//
// Version      : 1.0
// Last Updated : n/a
//-----------------------------------------------------------------------------

// Length of the arms
int l1 = 240;
int l2 = 240;

// Angles and coordinates of the arms
float theta1 = 0.0;
float theta2 = 0.0;
int x1;
int y1;
int x2;
int y2;

// The size each "step" is in radians
float MOTORSTEP;

// Points for the lines for calculating where the arms are
int px1 = 0;
int py1 = 0;
int px2;
int py2;
int px3;
int py3;

// List of commands
String[] command;

// List of times
String[] times;

// Laser variables
boolean laserEnabled = false;
int laserIntensity = 255;

// Determines how many frames are skipped in the calculations
int skipCount;

// Iterator value
int i = 0;

// Time count
long startTime = 0;
int waitTime = 0;

void setup() {
    // Reads in arguments or sets defaults
    if (args != null) {
        MOTORSTEP = float(args[1]);
        skipCount = int(args[2]);
    } else {
        MOTORSTEP = 0.05 * PI / 180.0;
        skipCount = 15;
    }

    // Initializes our canvas and reads in step.txt
    size(1080, 1000);
    background(0);
    frameRate(120);
    command = loadStrings("C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\step.txt");
    times = loadStrings("C:\\Users\\Admin\\Documents\\optiq-master\\src\\cpp\\res\\time.txt");

    // Shifts the origin to the bottom left
    scale(1, -1);
    translate(60, (60 + (l1+l2)/2) -height);

    // Draws an arc that represents the max cutting size
    noFill();
    stroke(255);
    strokeWeight(3);
    arc(l1 + l2, 0, 2 * (l1 + l2), 2 * (l1 +  l2), 0, PI);
    translate(l1 + l2, 0);  // Move the origin to the center of the arc

    // Draws the end of the "Table"
    strokeWeight(3);
    line(-width, 0, width, 0);
}

// Main loop
void draw() {
    // Moves the origin to be at the bottom center of the arc
    scale(1, -1);
    translate(60, (60 + (l1+l2)/2) -height);
    translate(l1 + l2, 0);

    for (int k = 0; k < skipCount; k++) {
    

    	// Parses the command string
        switch (command[i]) {
            case "0x1":
                theta1 += MOTORSTEP;
                break;
            case "0x3":
                theta1 -= MOTORSTEP;
                break;
            case "0x4":
                theta2 += MOTORSTEP;
                break;
            case "0xC":
                theta2 -= MOTORSTEP;
                break;

            case "0x5":
                theta1 += MOTORSTEP;
                theta2 += MOTORSTEP;
                break;
            case "0x7":
                theta1 -= MOTORSTEP;
                theta2 += MOTORSTEP;
            case "0xD":
                theta1 += MOTORSTEP;
                theta2 -= MOTORSTEP;
            case "0xF":
                theta1 -= MOTORSTEP;
                theta2 -= MOTORSTEP;

            case "0xA":
                laserEnabled = true;
                break;
            case "0xB":
                laserEnabled = false;
                break;
            case "0xE":
                i++;
                if (i > command.length - 1) {
                    exit();
                }
                laserIntensity = unhex(command[i].substring(2, command[i].length()));
        }

        // Calculates the values of the points of the arms
        px2 = (int)(cos(theta1) * l1);
        py2 = (int)(sin(theta1) * l1);
        px3 = (int)(cos(theta2 + theta1) * l2) + px2;
        py3 = (int)(sin(theta2 + theta1) * l2) + py2;
        
        //stroke(254);
        //line(px1,py1,px2,py2);
        //line(px2,py2,px3,py3);

        // Draws the path of the laser
        if (laserEnabled) {
          strokeWeight(1.2);
            stroke(0, 100, 50, laserIntensity);
        } else {
          strokeWeight(1.2);
            stroke(100, 50, 0, 100);
        }
        point(px3, py3);

        // End case
        i++;
        
        if (i > command.length-1) {
            noLoop();
            break;
        }
        
        waitTime();

    }
}

// Quit on keypress
void keyPressed() {
    exit();
}

void waitTime(){
    //background(0);
    waitTime = Integer.parseInt(times[i]);
    startTime = millis();
    println("here" + i); 
    while(millis() - startTime < waitTime) {           
      //wait      
    }
    redraw();
}
