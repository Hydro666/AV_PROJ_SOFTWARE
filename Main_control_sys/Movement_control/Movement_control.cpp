/*
 Name:		Movement_control.cpp
 Created:	11/24/2017 8:40:58 PM
 Author:	aquil
 Editor:	http://www.visualmicro.com
*/

#include "Movement_control.h"


// Executes all necessary setup values for the motors and sets a start speed 
void MOVEMENT::motor_setup(int start_speed) {
	// Motor control 
	// Motor control objects 
	AFMS = Adafruit_MotorShield(); 
	f_r = AFMS.getMotor(1);
	f_l = AFMS.getMotor(4);
	r_r = AFMS.getMotor(2);
	r_l = AFMS.getMotor(3);

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

// Accelerates the motors forward to a specified speed 
void MOVEMENT::motor_accel_fwd(int speed) {
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
void MOVEMENT::emergency_brake(int& speed) {
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

	advancement = 0; 
}