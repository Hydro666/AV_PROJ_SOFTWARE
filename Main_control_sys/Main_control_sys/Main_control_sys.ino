/*
 Name:		Main_control_sys.ino
 Created:	11/24/2017 
 Author:	Aquiles Gomez
 Description: 
	This file houses all the logic for the autonomous car project. 
	1. TODO: Create movement classes and include navigation within it 
	2. TODO: Create object detection 
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <sensor_control.h>
#include <Movement_control.h>

// Initial system setup  
MOVEMENT maneuver;
OBJECT_DETECTION object_prox_cl;
bool collision_check;
bool buffer_check;

void setup() {
	// Primary system start up 
	Serial.begin(9600); 
	maneuver.movement_setup(); 
	object_prox_cl.object_detection_begin(4.00); 

}

// The main function that executes the logic 
void loop() {
	Serial.print("test\n");
	// Check that there are no objects in front of the robot 
	collision_check = object_prox_cl.ObjectImmediatelyClose(); 
	buffer_check = object_prox_cl.ApproachingObjectShouldReduceSpeed(); 
	
	// For testing purposes: 
	// TODO: complete Test 1: 
	// Move the robot forwards until a collision might occur or the buffer is met, then stop
	// harhly or softly, respectively 
	if (collision_check != true) {
		Serial.print("moving\n");
		maneuver.fwd(1); 
	}
	else {
		Serial.print("stopping\n"); 
		maneuver.emergency_stop(1);
	}

	// TODO: Test 2: Go reverse for the same distance we went forward for then stop 

}
