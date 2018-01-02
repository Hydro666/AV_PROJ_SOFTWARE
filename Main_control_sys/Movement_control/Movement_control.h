/*
 Name:		Movement_control.h
 Created:	11/24/2017 8:40:58 PM
 Author:	Aquiles Gomez
 This library houses the movement control for the robot. 
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

class SPEED_CONTROLLER; 
class MOVEMENT; 


// Gets the required speed for the manuever and controls the power
// required for each motor  This value is between 0 - 255
class SPEED_CONTROLLER {
private:
	// Encoder for each wheel 
	ENCODER wheel_1; 
	ENCODER wheel_2; 
	ENCODER wheel_3; 
	ENCODER wheel_4; 

	// value we correct by (only used by the slave motors) 
	signed int correction_1;
	signed int correction_2; 
	int corrected_power_1;
	int corrected_power_2; 

	// current motor power
	int start_power;
	double speed_1; // master
	double speed_2; // slave
	double speed_3; // slave
	double speed_4; // master 

	double dis_1; 
	double dis_2; 
	double dis_3; 
	double dis_4; 
	
	// Returns speed so other functions can use it, requires at least one itteration
	// of begin() and end() to function properly 
	void get_speed_results();

	// Returns the distance values 
	void get_distance_results(); 

public:
	// Sets the encoder so that we may monitor the speed
	void start_speed_controller(); 

	// Sets the start power 
	int set_start_power(); 

	// Begins calculating speed from the encoder 
	void begin();
	
	// Gets a reading from the encoders. Useful during loops so this is called itterate. 
	void itterate(); 

	// Tells the encoder to stop calculates speed & distance 
	void end();

	// Checks to see if th erobot is moving staright. Returns 
	// TRUE if it is
	bool Moving_straight(); 

	// Sets corrected power. This requires a speed reading of some sort and 
	// retruns the 'corrected' values. slave number corresponds to the slave 
	// motor number, 1-> slave one, 2-> salve two 
	int set_corrected_power(int power, int slave); 
};

// Movement has a motor
// Each motor direction and speed
class MOVEMENT{
private:
	// Motor control 
	bool RobotIsMoving; 
	bool RobotIsGoingStraight; 
	bool RobotISMovingForward; 

	Adafruit_MotorShield AFMS;
	Adafruit_DCMotor *f_r;
	Adafruit_DCMotor *f_l;
	Adafruit_DCMotor *r_r;
	Adafruit_DCMotor *r_l;

	// Obstacle detection 
	OBJECT_DETECTION object; 

	// Power to each motor 
	SPEED_CONTROLLER power;
	int p1, p2, p3, p4; 
	int i, j, k, l; 

	// Sets all motors to run forward 
	void all_run_forward();

	// Sets ll motors to run backward 
	void all_run_backward(); 

public:
	// Motor setup 
	void movement_setup();

	// Accelerates the motors forward to a specified speed set by the speed controller 
	// Go straight Start from stop, Go straight already in motion,
	// direction (1 -> forward, 2-> backward)
	void fwd(int direction);

	// Stops the robot in case of a possible collision and keeps the robot stopped
	// we accelerate the motors in the opposite direction that we were going to counter 
	// momentum (1 -> we go back, 2 -> we go forward 
	void emergency_stop(); 

	// Gently pulls the robot to a stop unless an immediate object is detected
	// in front of the robot this is triggered by the speed controller 
	void buffer_stop();

};

#endif