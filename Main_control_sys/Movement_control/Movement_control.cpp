/*
 Name:		Movement_control.cpp
 Created:	11/24/2017 8:40:58 PM
 Author:	aquil
 Editor:	http://www.visualmicro.com
*/

#include "Movement_control.h"

// Motor control objects 
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *f_r = AFMS.getMotor(1);
Adafruit_DCMotor *f_l = AFMS.getMotor(4);
Adafruit_DCMotor *r_r = AFMS.getMotor(2);
Adafruit_DCMotor *r_l = AFMS.getMotor(3);

// Executes all necessary setup values for the motors and sets a start speed 
void motor_setup(int start_speed) {
	// Motor control setup 
	AFMS.begin();
	f_r->setSpeed(start_speed);
	f_l->setSpeed(start_speed);
	r_r->setSpeed(start_speed);
	r_l->setSpeed(start_speed);

	f_r->run(FORWARD);
	f_l->run(FORWARD);
	r_r->run(FORWARD);
	r_l->run(FORWARD);

	f_r->run(RELEASE);
	f_l->run(RELEASE);
	r_r->run(RELEASE);
	r_l->run(RELEASE);
}

// Checks to see if we have to begin stopping, taking in the distance 
// Returns TRUE if we have to begin to stop for either one of two conditions: 
// TODO: 1. We are approaching an obstacle and must begin stopping to not collide 
// TODO: 2. We approaching the target distance and must begin slowing to approach
// target
bool stop_collision() {
	// Check to see if we are approaching a collision with an object 
	// Keeping a buffer of at least 5 cm 

	
	// TODO: Until the short range sensor is implemented, 
	// this check only ensures that a buffer of 20 cm is maintained. 
	double read;
	read = read_distance_close(4);
	if(read < 21.00) {
		return true;
	}
	else {
		return false;
	}
}

// Accelerates the motors forward to a specified speed 
int motor_accel_fwd(int& speed, int& advancement) {
	// If we're starting from stopped
	if (advancement == 0) {
		f_r->run(FORWARD);
		f_l->run(FORWARD);
		r_r->run(FORWARD);
		r_l->run(FORWARD);
		for (int i = 0; i <= speed; i++) {
			f_r->setSpeed(i);
			f_l->setSpeed(i);
			r_r->setSpeed(i);
			r_l->setSpeed(i);
			delay(10);
		}
		advancement = 1; 
	}
	// If we're already in motion 
	if (advancement == 1) {
		f_r->setSpeed(speed);
		f_l->setSpeed(speed);
		r_r->setSpeed(speed);
		r_l->setSpeed(speed);
	}
}

// Cuts power to motors in case of imminent collison 
int emergency_stop(int& advancement, int& speed) {
	// Cuts power to motors 
	f_r->run(RELEASE);
	f_l->run(RELEASE);
	r_r->run(RELEASE);
	r_l->run(RELEASE);

	// Reverse backward to counter momentum. 
	f_r->run(BACKWARD);
	f_l->run(BACKWARD);
	r_r->run(BACKWARD);
	r_l->run(BACKWARD);
	for (int i = 0; i <= speed; i + 50) {
		f_r->setSpeed(i);
		f_l->setSpeed(i);
		r_r->setSpeed(i);
		r_l->setSpeed(i);
	}
	// Comes to a complete stop
	f_r->setSpeed(0);
	f_l->setSpeed(0);
	r_r->setSpeed(0);
	r_l->setSpeed(0);

	speed = 0; 
	advancement = 0; 
}