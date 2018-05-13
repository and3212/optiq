// Length of the arms
int l1 = 240;
int l2 = 240;

// Angles of the arms
float theta1 = 0.0;
float theta2 = 0.0;
int x1;
int y1;
int x2;
int y2;
float MOTORSTEP = (0.25*PI/180.0);
float addPi1;
float addPi2;

// Points for the lines to display the arms
int px1 = 0;
int py1 = 0;
int px2;
int py2;
int px3;
int py3;

String[] command;
boolean laserEnabled = false;
int laserIntensity = 255;

void setup() {
    size(1080, 1000);
    frameRate(240);
    command = loadStrings("step.txt");

    background(0);

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

// Loop values
int i = 0;

// Main loop
void draw() {
    scale(1, -1);
    translate(60, (60 + (l1+l2)/2) -height);
    translate(l1 + l2, 0);
/////////////////////////////////////////////////////////////////////

    //Math for angles
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


    // println(theta1 + addPi1);
    px2 = (int)(cos(theta1) * l1);
    py2 = (int)(sin(theta1) * l1);
    // println(theta2+addPi2);
    px3 = (int)(cos(theta2 + theta1) * l2) + px2;
    py3 = (int)(sin(theta2 + theta1) * l2) + py2;

///////////////////////////////////////////////////////////

    if (laserEnabled) {
        stroke(0, 100, 50, laserIntensity);
    } else {
        stroke(100, 50, 0, 100);
    }

    point(px3, py3);

    i++;
    if (i > command.length-1) {
        delay(5000);
        exit();
    }
}
