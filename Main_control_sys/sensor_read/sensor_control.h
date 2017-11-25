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

// Checks if the sensors have been previously calibrated 
bool is_previously_calibrated(double c1_c, double c2_c, double c1_f, double c2_f); 

// Reads values
double print_values(double c1_c, double c2_c, double c1_f, double c2_f);

// Delays x seconds 
void delay_x_secs(int x);

// Write to EEPROM
void write_to_memory(double c1_c, double c2_c, double c1_f, double c2_f); 

// Calibrates both the IR sensors and assigns calibration values 
float calibrate(double& c1_c, double& c2_c, double& c1_f, double& c2_f, int pin, int pin_2);

// Reads the distance from the GP2Y0A02YK sensor and returns the approximate distance in cm 
double read_distance_close(double c1_c, double c2_c, int pin, int readings = 5);

// Reads the distance from the GP2Y0A710K0F sensor and returns the approximate distance in cm
double read_distance_far(double c1_f, double c2_f, int pin, int readings = 5); 

#endif