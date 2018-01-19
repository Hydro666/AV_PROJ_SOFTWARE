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
	int correction_1;
	int correction_2; 
	int corrected_power_1;
	int corrected_power_2; 

	// current motor power
	int start_power;
	double speed_1; 
	double speed_2; 
	double speed_3; 
	double speed_4;  

	double dis_1; 
	double dis_2; 
	double dis_3; 
	double dis_4; 
	
	// Returns speed so other functions can use it, requires at least one itteration
	// of begin() and end() to function properly 
	void get_speed_results();

	// Returns the distance travled by each wheel  
	void get_distance_results(); 

public:
	// Sets the encoder so that we may monitor the speed
	void start_speed_controller(); 

	// Sets the start power if the robot is requested to go forward
	int set_fwd_start_power(); 
	
	// Sets the start power if the robot is requested to spin
	int set_spin_start_power(); 

	// Begins calculating speed from the encoder 
	void begin();
	
	// Gets a reading from the encoders. Useful during loops so this is called itterate. 
	void itterate(); 

	// Tells the encoder to stop calculates speed & distance 
	void end();

	// Resets all the wheel encoders. This is used when the robot starts a new maneuver
	// This resets speed and distance to zero 
	void reset(); 

	// Checks to see if Master and slave are equal . Returns 
	// TRUE if it is
	bool MasterSlaveSame(bool); 

	// Sets corrected power. This requires a speed reading of some sort and 
	// retruns the 'corrected' values. slave number corresponds to the slave 
	// motor number, 1-> slave one, 2-> salve two 
	int set_corrected_power(int power, int slave); 

	// Returns the amount the robot has turned in degrees. Rounds up to the nearest
	// whole number. This is only used during the spin maneuver 
	int degrees_spun(); 

	// Returns the approximate robot speed 
	double approximate_robot_speed(); 
};

// Movement has a motor
// Each motor direction and speed
class MOVEMENT{
private:
	// Motor control 
	bool RobotIsMoving; 
	bool RobotISMovingForward; 
	bool RobotIsSpinning; 

	Adafruit_MotorShield AFMS;
	Adafruit_DCMotor *f_r;
	Adafruit_DCMotor *f_l;
	Adafruit_DCMotor *r_r;
	Adafruit_DCMotor *r_l;

	// Obstacle detection 
	OBJECT_DETECTION object; 

	// Way point finder, used during spin detection 

	// Power to each motor 
	SPEED_CONTROLLER power;
	int p1, p2, p3, p4; 
	int i, j, k, l; 
	double s1, s2, s3, s4; 

	// Used to calculate the spin distance
	int degree; 

	// Sets all motors to run forward 
	void all_run_forward();

	// Sets all motors to run backward 
	void all_run_backward(); 

public:
	// Motor setup 
	void movement_setup();

	// Accelerates the motors forward to a specified speed set by the speed controller 
	// Go straight Start from stop, Go straight already in motion,
	// direction (1 -> forward, 2-> backward)
	void fwd(bool);

	// Tells the robot to spin in its current location. It should not move forward or
	// backward as it performs this maneuver. This function continues until 
	// the robot is told to stop. It keeps track of the amount of degree it has spun. 
	// Spins right if bool is true, left if not
	void spin(bool); 

	// Stops the robot in case of a possible collision and keeps the robot stopped
	// we accelerate the motors in the opposite direction that we were going to counter 
	// momentum (1 -> we go back, 2 -> we go forward 
	void emergency_stop(); 

	// Gently pulls the robot to a stop unless an immediate object is detected
	// in front of the robot this is triggered by the speed controller 
	void buffer_stop();

};

#endif