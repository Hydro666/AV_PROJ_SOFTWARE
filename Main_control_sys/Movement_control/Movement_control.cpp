/*
 Name:		Movement_control.cpp
 Created:	11/24/2017 8:40:58 PM
 Author:	Aquiles Gomez
*/

#include "Movement_control.h"

void SPEED_CONTROLLER::start_speed_controller() {
	Serial.print(F("Configuring speed controllers")); 
	wheel_1.encoder_begin(30); 
	wheel_2.encoder_begin(31); 
	wheel_3.encoder_begin(32); 
	wheel_4.encoder_begin(33); 
	Serial.print(F("Speed controllers configured")); 
}

// TODO: Implement speed controller / better determination for the power 
// Depending on closest object. 
int SPEED_CONTROLLER::set_fwd_start_power() {
	return 150;
}

// TODO: Implement speed controller / better determination for spin power depending
// if the robot needs to maneuver in a tight space 
int SPEED_CONTROLLER::set_spin_start_power() {
	return 50;
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

bool SPEED_CONTROLLER::MasterSlaveSame(bool RobotIsSpinning) {
	// Compare both the master and salves to see if there is an error 
	// add a small correction everytime we see there's an error and set
	// a new correction power. 
	double error_1; 
	double error_2; 
	if (RobotIsSpinning) {
		error_1 = speed_1 - speed_3;
		error_2 = speed_2 - speed_4; 
	}
	else {
		error_1 = speed_1 - speed_2;
		error_2 = speed_4 - speed_3;
	}

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

int SPEED_CONTROLLER::degrees_spun() {
	// Width of vehicle in cm
	double vehicle_width = 15.24;
	// Degress moved 
	int theta_1, theta_2; 

	// Equation used to calculate the amount of degrees moved
	// by the robot. This was obtained utilizing the arch length 
	// function. We only check for the distance moved by each of masters 
	// and return the average of each

	theta_1 = round((180 * dis_1) / (PI * vehicle_width)); 
	theta_2 = round((180 * dis_2) / (PI * vehicle_width)); 
	return (theta_1 + theta_2) / 2; 
}


void MOVEMENT::movement_setup() {
	Serial.print(F("Configuring Maneuver controls.\n")); 
	AFMS = Adafruit_MotorShield();
	f_r = AFMS.getMotor(3);
	f_l = AFMS.getMotor(4);
	r_r = AFMS.getMotor(2);
	r_l = AFMS.getMotor(1);

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
	power.start_speed_controller();
	object.configure_object_detection(4.00);
	RobotIsMoving = false; 
	RobotISMovingForward = false;
	RobotIsSpinning = false; 
	Serial.print(F("Maneuver configuration complete!\n"));
}

void MOVEMENT::all_run_forward() {
	f_r->run(FORWARD);
	f_l->run(FORWARD);
	r_r->run(FORWARD);
	r_l->run(FORWARD);
	RobotISMovingForward = true; 
}

void MOVEMENT::all_run_backward() {
	f_r->run(BACKWARD);
	f_l->run(BACKWARD);
	r_r->run(BACKWARD);
	r_l->run(BACKWARD);
	RobotISMovingForward = false;
}

void MOVEMENT::fwd(bool fwd) {
	// Check if robot is already in motion, if so we do not check the direction as
	// we assume we do not need to change direction rapidlly without stopping
	if (RobotIsMoving) {
		power.begin(); 
		f_r->setSpeed(p1); 
		f_l->setSpeed(p2); 
		r_r->setSpeed(p3); 
		r_l->setSpeed(p4); 
		power.itterate();
		// Check for collision
		if (object.ObjectImmediatelyClose()) {
			emergency_stop();
		}
		if (object.ObjectInBufferRange()) {
			buffer_stop();
		}
		power.end(); 
		power.MasterSlaveSame(RobotIsSpinning);
		p2 = power.set_corrected_power(p2, 1);
		p3 = power.set_corrected_power(p3, 2);
	}
	else {
		// If starting from stopped, we get intial speed values from the speed controller
		// and accelerate to said speed, correcting for possible inconsistencies as we 
		// accelerate. After we accelerate we check to see if we are moving consistently
		// if not we let the robot know so that it corrects it during the loop in the next 
		// run 
		if (fwd) {
			all_run_forward();
		}
		else {
			all_run_backward(); 
		}
		// Get intial power for each motor 
		p1 = power.set_fwd_start_power();
		p2 = power.set_fwd_start_power();
		p3 = power.set_fwd_start_power();
		p4 = power.set_fwd_start_power();
		RobotIsMoving = true;
		power.begin();

		for (i = 0, j = 0, k = 0, l = 0;
			i < p1, j < p2, k < p3, l < p4;
			i++, j++, k++, l++) {
			// check for collision, this prevents the robot from colliding as its 
			// accelerating forward 
			if (object.ObjectImmediatelyClose()) {
				emergency_stop();
				break; 
			}
			if (object.ObjectInBufferRange()) {
				buffer_stop();
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
		power.MasterSlaveSame(RobotIsSpinning);
		p2 = power.set_corrected_power(p2, 1);
		p3 = power.set_corrected_power(p3, 2);
	}
}

void MOVEMENT::spin(bool direction) {

	if (RobotIsSpinning){
		power.begin(); 
		f_r->setSpeed(p1);
		f_l->setSpeed(p2);
		r_r->setSpeed(p3);
		r_l->setSpeed(p4);
		power.itterate(); 
		degree = power.degrees_spun(); 
		power.end(); 
		power.MasterSlaveSame(RobotIsSpinning);
		p3 = power.set_corrected_power(p3, 1);
		p4 = power.set_corrected_power(p4, 1);
		}
	else {
		// Spins right if bool input is true, left if not 
		if (direction) {
			f_r->run(RELEASE);
			f_l->run(FORWARD);
			r_r->run(RELEASE);
			r_l->run(FORWARD);
		}
		else {
			f_r->run(FORWARD);
			f_l->run(RELEASE);
			r_r->run(FORWARD);
			r_l->run(RELEASE);
		}
		RobotIsSpinning = true; 
		// Get spin speed from the controller 
		p1 = power.set_spin_start_power(); 
		p2 = power.set_spin_start_power(); 
		p3 = power.set_spin_start_power(); 
		p4 = power.set_spin_start_power(); 
		power.begin(); 
		// Get total distance spun to calculate to degree of motion and assign it to 
		// the degree value so the robot knows how long it's moved. 
		for (i = 0, j = 0, k = 0, l = 0; i < p1, j < p2, k < p3, l < p4;
			i++, j++, k++, l++) {
			f_l->setSpeed(p2); 
			r_l->setSpeed(p4); 
			power.itterate(); 
			// Calculate degrees traveled by the robot 
			degree = power.degrees_spun(); 
		}
		// Apply correction 
		power.end(); 
		power.MasterSlaveSame(RobotIsSpinning);
		p3 = power.set_corrected_power(p3, 1); 
		p4 = power.set_corrected_power(p4, 1); 
	}
}

void MOVEMENT::emergency_stop() {
	Serial.print("Emergency brake applied.\n"); 
	if (RobotIsMoving) {
		// Apply a rapid acceleration backward to counter momentum using the current power
		// but in the opposite direction 
		if (RobotISMovingForward) {
			all_run_backward();
		}
		if (RobotISMovingForward != true) {
			all_run_forward();
		}

		f_r->setSpeed(i-10);
		f_l->setSpeed(j-10);
		r_r->setSpeed(k-10);
		r_l->setSpeed(l-10);

		f_r->setSpeed(0);
		f_l->setSpeed(0);
		r_r->setSpeed(0);
		r_l->setSpeed(0);
		RobotIsMoving = false; 
	}
	else {
		f_r->setSpeed(0);
		f_l->setSpeed(0);
		r_r->setSpeed(0);
		r_l->setSpeed(0);
	}
		
}

void MOVEMENT::buffer_stop() {
	if (RobotIsMoving) {
		// Get the current itteraton and slow down gently then stop 
		int amt = 50;
		all_run_backward(); 
		if (object.ObjectImmediatelyClose()) {
			emergency_stop();
		}
		f_r->setSpeed(i-amt);
		f_l->setSpeed(j-amt);
		r_r->setSpeed(k-amt);
		r_l->setSpeed(l-amt);

		f_r->setSpeed(0);
		f_l->setSpeed(0);
		r_r->setSpeed(0);
		r_l->setSpeed(0);
		RobotIsMoving = false;
	}
	if (RobotIsSpinning) {
		f_r->setSpeed(0);
		f_l->setSpeed(0);
		r_r->setSpeed(0);
		r_l->setSpeed(0);
		RobotIsSpinning = false;
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
