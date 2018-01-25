/*
Name:		Main_control_sys.ino
Created:	11/24/2017
Author:	Aquiles Gomez, Henry Lancelle
Description:
This file houses all the logic for the autonomous car project.
*/

// TODO: Improve memory allocation within all modules
// TODO: Expand on cautionary yield returns within the object detection modules 
#include "sensor_control.h"
#include <Movement_control.h>


namespace hardware {

/* Sensor allocations.*/
AnalogSensor irFar;
AnalogSensor irClose;
DigiSensor d1;
DigiSensor d2;
Encoder eFL, eFR, eRL, eRR;

SensorControl controller(irFar, irClose, d1, d2, eFL, eFR, eRL, eRR);

void setup() {
    // Primary system start up
    irFar.attach(AIrFar);
    irClose.attach(AIrClose);
    d1.attach()
    Serial.begin(9600);
}


// The main function that executes the logic 
void loop() {
    // For testing purposes:
    // Move the robot forwards until a collision might occur or the buffer is met, then stop
    // harhly or softly, respectively. Stay stopped if we are within buffer range.


}
} //hardware