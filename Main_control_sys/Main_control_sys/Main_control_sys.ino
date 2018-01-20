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

void setup() {
	// Primary system start up 
	Serial.begin(9600);
}


// The main function that executes the logic 
void loop() {
	// For testing purposes: 
	// Move the robot forwards until a collision might occur or the buffer is met, then stop
	// harhly or softly, respectively. Stay stopped if we are within buffer range. 


}