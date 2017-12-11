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

// Navigation has a way point, has a direction, has a movement 

// Movement has a motor
class MOVEMENT{
private:
	int advancement; 
	Adafruit_MotorShield AFMS;
	Adafruit_DCMotor *f_r; 
	Adafruit_DCMotor *f_l; 
	Adafruit_DCMotor *r_r; 
	Adafruit_DCMotor *r_l; 

public:
	// Motor setup 
	void motor_setup(int start_speed);
	// Accelerates the motors forward to a specified speed or keeps current
	// current movement 
	void motor_accel_fwd(int speed);
	// Applies brakes in case of a possible collision detection
	void emergency_brake(int& speed);
};

#endif