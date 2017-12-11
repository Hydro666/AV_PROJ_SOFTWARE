/*
 Name:		sensor_control.h
 Created:	11/14/2017 9:18:13 PM
 Author:	Aquiles Gomez
*/

#ifndef _sensor_control_h
#define _sensor_control_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <math.h>
#include <EEPROM.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


// Analog sensor control 
class IR_SENSOR {

private: 
	double c0;			// These are the calibration values for the senor 
	double c1;
	double volt_convert;
	int sensor; 

	
public: 

	// Functions
	void pin_setup(int pin, double volt);

	// Calibrate: 0 if we want to calibrate
	// 1 if we don't (close), 2 if we don't (far)
	void calibrate(int n);

	// Read sensor values
	double read_close();
	double read_far();

};

// Digital sensor control
class DIGI_SENSOR {

private: 
	int sensor; 

public: 
	void pin_setup(int pin); 

	// Returns TRUE if an object is detected in front of the 
	// sensor 
	bool ObjectIsToClose(); 
};

#endif
