/*
 Name:		sensor_control.cpp
 Created:	11/14/2017 9:18:13 PM
 Author:	Aquiles Gomez
*/

#include "sensor_control.h"

void IR_SENSOR::ir_begin(int pin, double volt) {
	sensor = pin; 
	volt_convert = volt / 1024; 
	pinMode(sensor, INPUT); 
}

void IR_SENSOR::calibrate(int n) {

	// Starts calibration process 
	if (n == 0) {
		// Far
		int duration = 1000;
		Serial.print("Far calibration...\n");
		delay(2000);

		int sensor_cumulative = 0;
		int reads = 0;
		unsigned long timeStart = millis();
		do {
			delay(20);
			sensor_cumulative += analogRead(sensor);
			reads++;
		} while (millis() <= timeStart + duration);
		c0 = sensor_cumulative / reads;

		Serial.print(c0);
		Serial.println();

		// Close 
		Serial.print("Close calibration");
		delay(2000);

		sensor_cumulative = 0;
		reads = 0;
		timeStart = millis();
		do {
			delay(20);
			sensor_cumulative += analogRead(sensor);
			reads++;
		} while (millis() <= timeStart + duration);
		c1 = sensor_cumulative / reads;

		Serial.print(c1);
		Serial.println();
	}

	// If we do not want to calibrate, we assume ideal values for the 
	// close range sensor
	if (n == 1) {
		c0 = 65; 
		c1 = 372; 
	}
	
	// If we do not want to calibrate, we assume ideal values for the 
	// far range sensor 
	if (n == 2) {
		c0 = 211; 
		c1 = 454; 
	}
}

double IR_SENSOR::read_close() {
	double cumulative_value = 0;  
	double cal_value;
	int readings = 5; 

	// Obtain the input over a number of readings 
	for (int i = 0; i < readings; i++) {
		delay(20);
		cumulative_value += analogRead(sensor);
	}

	// The distance is calculated using data points from the GP2Y0A02YK data sheet
	// This equation relates the distance vs the voltage reading. This is a power function with 
	// an R^2 value == .9961
	// We scale the values we read between 65  and 372 as these are the absolute minimums 
	// (per datasheet), utilizing the calibrated values to account for ambient lighting 
	cal_value = map((cumulative_value / readings), c0, c1, 65, 372) * volt_convert;

	return (62.1418 * pow(cal_value, -1.115));
}

double IR_SENSOR::read_far() {
	double cumulative_value = 0; 
	double cal_value;
	int readings = 5; 

	// Obtain the readings 
	for (int i = 0; i < readings; i++) {
		delay(20);
		cumulative_value += analogRead(sensor);
	}

	// The distance is calcualted from the GP2Y0A710K0F data sheet. Power function with
	// R^2 == .973 Readings are scaled between 211 and 454 as these are the absolute 
	// minimums and maximums respectively (datasheet) utilizing the calibrated values 
	// to account for ambient lighting 
	cal_value = map((cumulative_value / readings), c0, c1, 211, 454) * volt_convert;

	return (1199.55 * pow(cal_value, -2.833));
}

void DIGI_SENSOR::digi_begin(int pin) {
	sensor = pin; 
	pinMode(sensor, INPUT); 
}
 
bool DIGI_SENSOR::ObjectIsToClose() {
	int val = 0;
	val = digitalRead(sensor);
	if (val == 1) {
		return false;
	}
	if (val == 0) {
		return true;
	}
}


void ENCODER::encoder_begin(int pin) {
	sensor = pin;
	pinMode(sensor, INPUT);
	wheel_circumference = .20420352248;
}

void ENCODER::begin_speed_calc() {
	timeStart = millis();
	tick = 0;
	wheel_rotations = 0;
	dis_traveled = 0;
}

void ENCODER::read_value() {
	value = digitalRead(sensor);
	if (value == 0) {
		tick++;
	}
}

void ENCODER::end_speed_calc() {
	timeEnd = millis();
	time = (timeEnd - timeStart) / 1000; 
	wheel_rotations = tick / 20; 
	distance = wheel_rotations * wheel_circumference; 
	dis_traveled += distance; 
	speed = distance / time; 
}

double ENCODER::get_speed() {
	return speed; 
}

double ENCODER::get_distance() {
	return dis_traveled; 
}


void OBJECT_DETECTION::object_detection_begin(double volt) {
	far.ir_begin(14, volt);
	close.ir_begin(13, volt); 
	fwd.digi_begin(44); 
	bck.digi_begin(45); 
}


bool OBJECT_DETECTION::ObjectImmediatelyClose() {
	// We check if a digital sensor has fired and note the position of the object 
	if (fwd.ObjectIsToClose()) {
		ObjectIsInFront = true;
		return true;
	}
	if (bck.ObjectIsToClose()) {
		ObjectIsBehind = true;
	}
	else {
		ObjectIsInFront = false; 
		ObjectIsBehind = false; 
		return false; 
	}
}


bool OBJECT_DETECTION::ApproachingObjectShouldReduceSpeed() {
	double limit = 40.00; 
	double last_four_reads[4];
	for (int i = 0; i < 4; i++) {
		last_four_reads[i] = close.read_close();
	}

	// Check to see if the values are decreasing, if they are, check the last read
	// to see if it is within the limit Return TRUE if they are within the 
	// the acceptable limit 
	if (ArrayIsDecreasing(last_four_reads, 4)) {
		if (last_four_reads[3] <= limit) {
			return true; 
		}
		else {
			return false;
		}
	}
	else {
		return false; 
	}
}