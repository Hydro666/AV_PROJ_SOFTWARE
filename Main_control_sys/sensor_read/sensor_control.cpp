/*
 Name:		sensor_read.cpp
 Created:	11/14/2017 9:18:13 PM
 Author:	Aquiles Gomez
*/

#include "sensor_control.h"


void IR_SENSOR::pin_setup(int pin, double volt) {
	sensor = pin;
	volt_convert = volt / 1024; 
	pinMode(sensor, INPUT);
}

// IR Sensor calibration 
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

// Function that obtains the approximate distance from the close range sensor.
// We average out from a number of readings to obtain a more consistent result. 
// The number of readings can be tweaked to increase performance or to make 
// the reading more accurate. Returns current distance value from the close range sensor 
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

// Function that obtains the approximate distance of the long range sensor. 
// We average our readings to obtain a more consistent result. The number 
// of readings can be tweaked to increase performance or to make the 
// readings more accurate. Returns current distance value from long range sensor
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

void DIGI_SENSOR::pin_setup(int pin) {
	sensor = pin;
	pinMode(sensor, INPUT);
}

// Checks to see if an object is detected by the very close range sensor
// Returns TRUE if an object is detected. 
bool DIGI_SENSOR::ObjectIsDetectedToClose() {
	int val = 0;
	val = digitalRead(sensor);
	if (val == 1) {
		return false;
	}
	if (val == 0) {
		return true;
	}
}
