/*
 Name:		Main_control_sys.ino
 Created:	11/24/2017 
 Author:	Aquiles Gomez, Henry Lancelle
 Description: 
	This file houses all the logic for the autonomous car project. 
*/

// TODO: Improve memory allocation within all modules
// TODO: Expand on cautionary yield returns within the object detection modules 

#include <sensor_control.h>
#include <Movement_control.h>

// Initial system setup

int object_distance;
double sensor_voltage = 4.00;
void setup() {
	// Primary system start up 
	Serial.begin(9600); 
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

}
