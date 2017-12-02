/*
 Name:		Movement_control.h
 Created:	11/24/2017 8:40:58 PM
 Author:	aquil
 Editor:	http://www.visualmicro.com
*/

#ifndef _Movement_control_h
#define _Movement_control_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Adafruit_Sensor.h>
#include <sensor_control.h>

// Motor setup 
void motor_setup(int start_speed);

// Checks to see if we must perform an immediate stop due to buffer violation 
bool stop_collision();

// Check to see if we have to stop due to approaching target distance 

// Accelerates the motors forward to a specified speed 
int motor_accel_fwd(int& speed, int& advancement); 

// Cuts power to motors in case of imminent collison 
int emergency_stop(int& advancement, int& speed);

#endif