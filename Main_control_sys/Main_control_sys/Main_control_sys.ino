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

// Pins: 
// close =  A9
// Far = A8
// rear close = 42
// farward close = 44

// Initial system setup  
// Define the pins used for input 
IR_SENSOR close;
IR_SENSOR far;
DIGI_SENSOR fwd;
DIGI_SENSOR rear;

void setup() {
	// Begin
	Serial.begin(9600); 
	close.pin_setup(A9, 6.8); 
	far.pin_setup(A8, 6.8); 
	fwd.pin_setup(44); 
	rear.pin_setup(42); 

	// Calibration 
	close.calibrate(1); 
	far.calibrate(2); 

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
	// Test 1: 
	// Move the robot forwards until a collision might occur, then stop 


}
