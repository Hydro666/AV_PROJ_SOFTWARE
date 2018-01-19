/*
 Name:		sensor_control.h
 Created:	11/14/2017 9:18:13 PM
 Authors:	Aquiles Gomez, Henry Lancelle 

 This header file houses the controls for the proximity sensors as well 
 as the wheel encoders. It also contains the class for the object detection 
 module which is used to determine if there are any objects near the robot
 for which the robot must react. 
*/

#ifndef _sensor_control_h
#define _sensor_control_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


#include <math.h>
#include "Util.h"
#include "AbstractSensor.h"

class IR_SENSOR; 
class DIGI_SENSOR; 
class ENCODER; 
class OBJECT_DETECTION; 

// Pin loctaions for each sensor: 
/* 
	Digital Sensors: 
		IR-fwd: 44
		IR- r:  45 

	Analog sensors:
		IR far = A13
		IR close = A14

		Encoder 
			front:
				left = 30
				right = 31 
			back: 
				left = 32
				right = 33
	*/

// Analog sensors control 
class IR_SENSOR /*: AbstractSensor*/ {

private: 
	double c0;			// These are the calibration values for the senor 
	double c1;
	double volt_convert;
	int readings;
	int sensor; 

public: 
	void ir_begin(int, double); 


	// Calibrate: 0 if we want to calibrate
	// 1 if we don't (close), 2 if we don't (far)
	void calibrate(int);

	// Function that obtains the approximate distance from the close range sensor.
	// We average out from a number of readings to obtain a more consistent result. 
	// The number of readings can be tweaked to increase performance or to make 
	// the reading more accurate. Returns current distance value from the close range sensor 
	int read_close();

	// Function that obtains the approximate distance of the long range sensor. 
	// We average our readings to obtain a more consistent result. The number 
	// of readings can be tweaked to increase performance or to make the 
	// readings more accurate. Returns current distance value from long range sensor
	int read_far();

};

// Digital sensors control
class DIGI_SENSOR {

private: 
	int sensor; 

public: 
	void digi_begin(int); 

	// Checks to see if an object is detected by the very close range sensor
	// Returns TRUE if an object is detected.
	bool ObjectIsToClose(); 
};

// Speed encoders for the motors 
class ENCODER {

private: 
	int sensor; 
	int value;					
	int tick; 
	double speed;				// in m/s
	double distance;			// in meters 
	double dis_traveled;		// in meters 
	double wheel_rotations; 
	double wheel_circumference; // in meters 
	unsigned long time;			// in seconds
	unsigned long timeStart;	// in milliseconds
	unsigned long timeEnd; 
public: 
	// Sets up the encoder
	void encoder_begin(int);

	// Begins the speed counting procedure. This is done at the beginning of each itteration to 
	// tell the encoder to start reading data. The speed calcualtion is run after each loop. We 
	// count how long each loop took to determine the robot's current speed
	void begin_speed_calc();

	// Reads the values from the encoder
	void read_value(); 

	// Calculates speed and distance 
	void end_speed_calc();

	// Returns the speed obtained from the encoder in m/s
	double get_speed(); 

	// Returns the distance traveled in meters 
	double get_distance(); 

	// Sets the intial values for the encoder. Very useful when we start a new maneuever
	// and we want to track how much a particular maneuver moves the robot
	void reset();
};

// Class that detects obstacles around the robot. This allows the robot to 
// manever around obstacles 
class OBJECT_DETECTION {
private:
	
	// Sensors used for detection 
	IR_SENSOR far; 
	IR_SENSOR close; 
	DIGI_SENSOR fwd; 
	DIGI_SENSOR bck; 

	// IR sensor limits 
	const int far_upper_bound = 550; 
	const int far_lower_bound = 100;
	const int close_upper_bound = 120; 
	const int close_lower_bound = 20; 
	const int close_blind_spot_limit = 30;

	// Adjust far_read and close_read if we change this value 
	// See lines 164, 165
	const int read_number = 3;

	bool ObjectMayBeInBlindspot;
	bool ObjectInCloseSensorRange; 
	bool ObjectInFarSensorRange; 

	// Object status 
	bool ObjectIsLikelyStatic; 
	bool ObjectIsMovingTowards; 
	bool ObjectIsMovingAway; 

	// Readings from the sensors are stored here. 
	int close_read[3];
	int far_read[3];
	ARRAY close_analysis;
	ARRAY far_analysis;
 
	bool SensorOverLapExists(); 
	bool ObjectInBlindSpot(); 

public: 
	// Starts object detection: volt 
	void configure_object_detection(int);

	// Reports the distance of the closest object and informs the robot 
	// if it must proceed with caution if there might be an object out of the
	// detection range (blind spot). 
	int closest_object_distance();

	// Reports the speed of the currently detected object and whether it is moving toward 
	// or away from the robot. Speed is in m/s
	double detected_object_speed(unsigned long);

	// Object maneuvers
	bool ObjectApproaching(); 
	bool ObjectLeaving(); 
	bool ObjectStatic(); 

	// TODO: Expand on the returns for object yield, (Object Is approaching for
	// example) 
	// Returns True if the robot should be cautious due to an object detection
	// result 
	bool CautionRequiredDueToObject();
};

#endif