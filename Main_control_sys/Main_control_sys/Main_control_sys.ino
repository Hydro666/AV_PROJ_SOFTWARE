/*
 Name:		Main_control_sys.ino
 Created:	11/2/2017 
 Author:	Aquiles Gomez
 Description: 
	This file houses all the logic for the autonomous car project. 
	1. TODO: Create movement classes and include navigation within it 
	2. TODO: Create object detection 
*/
#include <EEPROM.h>
#include <sensor_control.h>

// Pin setup 
int far = A8; 
int close = A9; 
// IR sensor calibration values 
double c1_c = 0.00;
double c2_c = 3.64; 
double c1_f = 1.97;
double c2_f = 3.49;

 // Set up initial values 
void setup() {
	pinMode(far, INPUT);
	pinMode(close, INPUT);
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
		write_to_memory(c1_c, c2_c, c1_f, c2_f);
		print_values(c1_c, c2_c, c1_f, c2_f);
	}
	*/
}

// The main function that executes the logic 
void loop() {
}