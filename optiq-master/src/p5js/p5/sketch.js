// Length of the arms
var l1 = 240;
var l2 = 240;

// Angles of the arms
var theta1 = 0.0;
var theta2 = 0.0;
var x1;
var y1;
var x2;
var y2;
var MOTORSTEP = 0.05 * Math.PI / 180.0;
var addPi1;
var addPi2;

// Povars for the lines to display the arms
var px1 = 0;
var py1 = 0;
var px2;
var py2;
var px3;
var py3;

// Imports and parses text
var command;
var data;

// Laser variabes
var laserEnabled = false;
var laserIntensity = 255;

// Control structure variables
var fileLoaded = false;
var i = 0;


function setup() {
	// Sets up our default canvas and FPS
	var canvas = createCanvas(1080, 1000);
    background(0);
    frameRate(120);

	// Saves the current state of default translation
	push();
	
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

	// Slider to control the speed
	slider = createSlider(1, 100, 15, 1);
  	slider.position((windowWidth/2)-(150/2), windowHeight - 100);
  	slider.style('width', '150px');
	
	// Text input
	canvas.drop(readFile);
}

function draw() {
	// Shifts the origin to the center of the arc
    scale(1, -1);
    translate(60, (60 + (l1+l2)/2) -height);
    translate(l1 + l2, 0);

	if (fileLoaded) {
		for (var k = 0; k < slider.value(); k++) {

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
				    if (i > command.size - 1) {
				        remove();
					}
				    
				    laserIntensity = unhex(command[i].substring(2, command[i].length));
					break;
			}

			// Calculates the values of the points of the arms
			px2 = (int)(cos(theta1) * l1);
			py2 = (int)(sin(theta1) * l1);
			px3 = (int)(cos(theta2 + theta1) * l2) + px2;
			py3 = (int)(sin(theta2 + theta1) * l2) + py2;

			// Draws the path of the laser
			strokeWeight(2);
			if (laserEnabled) {
				stroke(0, 100, 50, laserIntensity);
			} else {
				stroke(100, 50, 0, 100);
			}
			point(px3, py3);


			// End case
			i++;
			if (i > command.size-1) {
				i = 0;
			}
		}
	}
}

// Reads files that are dragged onto the canvas
function readFile(file) {
	// Reset the origin
	pop();
	push();


	// Redraws the screen
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

	// Reset our variables
	i = 0;
	theta1 = 0;
	theta2 = 0;
	px1 = 0;
	py1 = 0;

	// Creates an array filled with the text data
	data = file.data;
	command = splitTokens(data, "\n");

	// Starts the main loop
	fileLoaded = true;
}

// If the window is resized, fix the slider position
function windowResized() {
	slider.position((windowWidth/2)-(150/2), windowHeight - 100);
}
