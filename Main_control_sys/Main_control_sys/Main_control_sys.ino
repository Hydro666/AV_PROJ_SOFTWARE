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
#include <HardwareProperties.h>

// Initial system setup  
MOVEMENT maneuver;
OBJECT_DETECTION object_prox_cl;
bool CollisionCheck;
bool Buffer;
int nearest_object_distance;

void setup() {
	// Primary system start up 
	Serial.begin(9600);
    pinMode(DirFwd, INPUT);
    pinMode(DirRev, INPUT);
    pinMode(AIrFar, INPUT);
	maneuver.movement_setup();

}


// The main function that executes the logic 
void loop() {
	// Check that there are no objects in front of the robot 
	CollisionCheck = object_prox_cl.ObjectImmediatelyClose();
	Buffer = object_prox_cl.ObjectInBufferRange();
	nearest_object_distance = object_prox_cl.object_distance_close();

	// For testing purposes: 
	// Move the robot forwards until a collision might occur or the buffer is met, then stop
	// harhly or softly, respectively. Stay stopped if we are within buffer range. 

	// Only move if are no objects in front of the robot 
	if (!Buffer) {
		Serial.print(F("Looks clear, proceeding forward\n"));
		Serial.print(F("Closest object is: "));
		Serial.print(nearest_object_distance);
		Serial.println();
		maneuver.fwd(true);
	}
	// If we reach the buffer distance or we are within buffer distance, we slow gently and come to a stop 
	if (Buffer) {
		Serial.print(F("Slowing due to object approaching. Distance is: \n"));
		Serial.print(nearest_object_distance);
		Serial.println();
		maneuver.buffer_stop();
	}

	// If we detect a collision, we ignore everything else and just come to a stop or remain stopeed. 
	if (CollisionCheck) {
		Serial.print(F("Collision imminent.\n"));
		maneuver.emergency_stop();
	}
}
