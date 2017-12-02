/*
 Name:		Main_control_sys.ino
 Created:	11/24/2017 
 Author:	Aquiles Gomez
 Description: 
	This file houses all the logic for the autonomous car project. 
	1. TODO: Create movement classes and include navigation within it 
	2. TODO: Create object detection 
*/

#include <sensor_control.h>
#include <Movement_control.h>

void setup() {
	// Sensor and motor setup
	ir_pin_setup(); 
	motor_setup(50);
	// Communication set up
	Serial.begin(9600);

	/* TODO: Include way to store calibrated values. This will make it faster to test the 
	robot if we are satisfied with the sensor calibraion. For testing purposes, the calibrated 
	values were obtained through testing and are implemented manually. This will eventually be 
	implemented with a harware implement (likely a switch) and with an LCD screen to display the 
	current calibration step and returns the calibrated values 

	// Check if the robot has been previously calibrated
	if (is_previously_calibrated(c1_c, c2_c, c1_f, c2_f)) {
		print_values(c1_c, c2_c, c1_f, c2_f);
	}
	else {
		calibrate(c1_c, c2_c, c1_f, c2_f, CLOSE, FAR);
	}
	*/
}

// The main function that executes the logic 
void loop() {
	// Movement logic
	// Each movement continues until it is told to stop or if the robot
	// is going to have a collision 
	// TODO: Create function that moves foward 
	// and stops when either the specified distance is met OR we are about 
	// to collide with an object 

	// For testing purposes: 
	// print the values read to determine how to effectively use stop_collision 
}
