/*
Name:		sensor_control.h
Created:	11/14/2017 9:18:13 PM
Author:	Aquiles Gomez, Henry Lancelle
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

#include <HardwareProperties.h>
#include <math.h>
#include "Util.h"

class HARDWARE;
class IR_CALCULATION;
class OBJECT_DETECTION; 

// Controls all of the sensor reading for the robot. Then returns said values 
// to be used in other functions
class HARDWARE {
	friend class IR_CALCULATION;
	friend class ENCODER_CALCULATION; 
private:
	// Ir sensors values
	struct analog_values {
		int IR_FAR;
		int IR_CLOSE;
	};
	struct digital_values {
		int FRONT;
		int REAR;
	};
	//Encoder values
	struct encoder_values {
		int F_R;
		int F_L;
		int R_R;
		int R_L;
	};

	double system_voltage;
	const int read_number = 5;
	analog_values ir_read;
	digital_values digi_read;
	encoder_values encoder_read;

	// Returns analog reading, 1 for far, 2 for close
	int get_analog_reading(int sensor);
	// Returns digital reading 1 for front, 2 for rear 
	int get_digital_reading(int sensor);
	// Returns encoder reading 1 for front right, 2 for front left,
	// 3 for rear right, 4 for rear left 
	int get_encoder_result(int sensor);

public:
	// Starts all the hardware 
	void Hardware_begin(double voltage);

	// Reads all sensor values and notes time of last reading
	void read_sensor_values();

};

// Interprets the results from ir sensor in the HARDWARE class
class IR_CALCULATION {
	friend OBJECT_DETECTION; 
private:
	// Returns the calcuated distance from the specified sensor 
	// 1 for far, 2 for close 
	int get_distance(HARDWARE& sensor_data, int sensor);

	// Returns true if either the front sensor or rear sensor reports an object 
	bool ObjectImmediatelyClose(HARDWARE& sensor_data, int sensor);
};

// Class that detects obstacles around the robot.
class OBJECT_DETECTION {
private:
	ARRAY close_analysis;
	ARRAY far_analysis;

	// Readings from the sensors are stored here. 
	int close_read[3];
	int far_read[3];

	// IR sensor limits
	enum sensor_constants : int {
		far_upper_bound = 550,
		far_lower_bound = 100,
		close_upper_bound = 120,
		close_lower_bound = 20,
		close_blind_spot_limit = 30,
		front = 1,
		rear = 2,
		far = 1,
		close = 2,
		rear = 2,
		read_number = 3
	};

	// Object Properties 
	bool ObjectMayBeInBlindspot;
	bool ObjectInCloseSensorRange;
	bool ObjectInFarSensorRange;

	bool SensorOverLapExists();
	bool ObjectInBlindSpot(HARDWARE& sensor_data, IR_CALCULATION& sensor_result);
public:

	// Reports the distance of the closest object and informs the robot 
	// if it must proceed with caution if there might be an object out of the
	// detection range (blind spot). 
	int closest_object_distance(HARDWARE& sensor_data, IR_CALCULATION& sensor_result);

	// Reports the speed of the currently detected object and whether it is moving toward 
	// or away from the robot. Speed is in m/s
	double detected_object_speed(unsigned long Time);

	// TODO: Expand on the returns for object yield, (Object Is approaching for
	// example) 
	// Returns True if the robot should be cautious due to an object detection
	// result 
	bool CautionRequiredDueToObject();

};

#endif