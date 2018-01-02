/*
 Name:		Movement_control.cpp
 Created:	11/24/2017 8:40:58 PM
 Author:	Aquiles Gomez
*/

#include "Movement_control.h"

												// 30,     31  ,      32        33
void SPEED_CONTROLLER::start_speed_controller() {
	wheel_1.encoder_begin(30); 
	wheel_2.encoder_begin(31); 
	wheel_3.encoder_begin(32); 
	wheel_4.encoder_begin(33); 
}

int SPEED_CONTROLLER::set_start_power() {
	start_power = 150;
	return start_power;
}

void SPEED_CONTROLLER::begin() {
	wheel_1.begin_speed_calc(); 
	wheel_2.begin_speed_calc(); 
	wheel_3.begin_speed_calc(); 
	wheel_4.begin_speed_calc(); 
}

void SPEED_CONTROLLER::itterate() {
	wheel_1.read_value(); 
	wheel_2.read_value(); 
	wheel_3.read_value(); 
	wheel_4.read_value();
}

void SPEED_CONTROLLER::end() {
	wheel_1.end_speed_calc(); 
	wheel_2.end_speed_calc(); 
	wheel_3.end_speed_calc(); 
	wheel_4.end_speed_calc(); 
	get_speed_results(); 
}

void SPEED_CONTROLLER::get_speed_results() {
	speed_1 = wheel_1.get_speed(); 
	speed_2 = wheel_2.get_speed(); 
	speed_3 = wheel_3.get_speed(); 
	speed_4 = wheel_4.get_speed(); 
}

void SPEED_CONTROLLER::get_distance_results() {
	dis_1 = wheel_1.get_distance(); 
	dis_2 = wheel_2.get_distance(); 
	dis_3 = wheel_3.get_distance(); 
	dis_4 = wheel_4.get_distance(); 
}

bool SPEED_CONTROLLER::Moving_straight() {
	// Compare both the master and salves to see if there is an error 
	// add a small correction everytime we see there's an error and set
	// a new correction power. 
	double error_1; 
	double error_2; 
	error_1 = speed_1 - speed_2; 
	error_2 = speed_4 - speed_3; 

	if ((error_1 || error_2) != 0.00) {
		if (error_1 > 0.00) {
			correction_1++;
		}
		if (error_1 < 0.00) {
			correction_1--;
		}
		if (error_2 > 0.00) {
			correction_2++;
		}
		if (error_2 < 0.00) {
			correction_2--;
		}
		return true;
	}
	else {
		return false;
	}
}

int SPEED_CONTROLLER::set_corrected_power(int power, int slave) {
	// We add the corrections to the slave motors 
	if (slave == 1) {
		corrected_power_1 = power + correction_1;
		return corrected_power_1;
	}
	if (slave == 2) {
		corrected_power_2 = power + correction_2; 
		return corrected_power_2; 
	}
}


void MOVEMENT::movement_setup() {
	Serial.print("Setting up motors...\n");
	// Motor control 
	// Motor control objects 
	AFMS = Adafruit_MotorShield();
	f_r = AFMS.getMotor(1);
	f_l = AFMS.getMotor(4);
	r_r = AFMS.getMotor(2);
	r_l = AFMS.getMotor(3);

	// Motor control setup 
	AFMS.begin();
	all_run_forward();

	f_r->setSpeed(0);
	f_l->setSpeed(0);
	r_r->setSpeed(0);
	r_l->setSpeed(0);

	f_r->run(RELEASE);
	f_l->run(RELEASE);
	r_r->run(RELEASE);
	r_l->run(RELEASE);

	// We assume we're stopped when starting and that the robot is moving straight 
	RobotIsMoving = false;
	power.start_speed_controller();
	object.object_detection_begin(4.00);
	Serial.print("Motor Setup Complete!\n");

}
void MOVEMENT::all_run_forward() {
	f_r->run(FORWARD);
	f_l->run(FORWARD);
	r_r->run(FORWARD);
	r_l->run(FORWARD);
}

void MOVEMENT::all_run_backward() {
	f_r->run(BACKWARD);
	f_l->run(BACKWARD);
	r_r->run(BACKWARD);
	r_l->run(BACKWARD);
}

void MOVEMENT::fwd(int direction) {
	// Check if robot is already in motion, if so we do not check the direction as
	// we assume we do not need to change direction rapidlly without stopping
	if (RobotIsMoving) {
		// Check for collision
		if (object.ObjectImmediatelyClose()) {
			emergency_stop(direction);
		}
		RobotIsGoingStraight = power.Moving_straight();
		p2 = power.set_corrected_power(p2, 1);
		p3 = power.set_corrected_power(p3, 2);
		f_r->setSpeed(p1); 
		f_l->setSpeed(p2); 
		r_r->setSpeed(p3); 
		r_l->setSpeed(p4); 
		Serial.print("Looks clear, so I'll keep moving.\n"); 
	}
	else {
		// If starting from stopped, we get intial speed values from the speed controller
		// and accelerate to said speed, correcting for possible inconsistencies as we 
		// accelerate. After we accelerate we check to see if we are moving consistently
		// if not we let the robot know so that it corrects it during the loop in the next 
		// run 
		if (direction == 1) {
			all_run_forward();
		}
		else {
			all_run_backward(); 
		}
		// Get intial power for each motor 
		p1 = power.set_start_power(); 
		p2 = power.set_start_power(); 
		p3 = power.set_start_power(); 
		p4 = power.set_start_power(); 
		power.begin(); 
		RobotIsMoving = true;

		for (i = 0, j = 0, k = 0, l = 0;
			i < p1, j < p2, k < p3, l < p4;
			i++, j++, k++, l++) {
			// check for collision, this prevents the robot from colliding as its 
			// accelerating forward 
			if (object.ObjectImmediatelyClose()) {
				emergency_stop(direction);
				break; 
			}
			f_r->setSpeed(i); 
			f_l->setSpeed(j);
			r_r->setSpeed(k); 
			r_l->setSpeed(l); 
			power.itterate();
		}
		power.end(); 

		// Apply small correction to robot and see if we are traveling straight
		// if not we let the robot know so that it can correct it during the next
		// itteration. Also let the robot know that it is moving at this point 
		RobotIsGoingStraight = power.Moving_straight();
		p2 = power.set_corrected_power(p2, 1);
		p3 = power.set_corrected_power(p3, 2);
	}
}

void MOVEMENT::emergency_stop(int direction) {
	f_r->run(RELEASE); 
	f_l->run(RELEASE); 
	r_r->run(RELEASE); 
	r_l->run(RELEASE); 
	Serial.print("Emergency brake applied.\n"); 
	if (RobotIsMoving) {
		// Apply a rapid acceleration backward to counter momentum using the current power
		// but in the opposite direction 
		if (direction == 1) {
			all_run_backward();
		}
		if (direction == 2) {
			all_run_forward();
		}

		f_r->setSpeed(i-10);
		f_l->setSpeed(j-10);
		r_r->setSpeed(k-10);
		r_l->setSpeed(l-10);
		RobotIsMoving = false;
	}
	else {
		f_r->setSpeed(0);
		f_l->setSpeed(0);
		r_r->setSpeed(0);
		r_l->setSpeed(0);
		RobotIsMoving = false; 
	}
		
}