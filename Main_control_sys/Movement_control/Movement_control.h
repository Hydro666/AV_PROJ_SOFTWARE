/*
 Name:		Movement_control.h
 Created:	11/24/2017 8:40:58 PM
 Author:	Aquiles Gomez, Henry Lancelle
 This library houses the movement control for the robot. 
*/

#ifndef _Movement_control_h
#define _Movement_control_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <HardwareProperties.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Adafruit_Sensor.h>
#include <sensor_control.h>
#endif