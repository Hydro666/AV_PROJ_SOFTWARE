/*
Name:		sensor_control.cpp
Created:	11/14/2017 9:18:13 PM
Author:	Aquiles Gomez, Henry Lancelle
*/

#include "sensor_control.h"

void HARDWARE::Hardware_begin(double voltage) {
	// Sets system voltage 
	system_voltage = voltage;

	// Set as inputs 
	pinMode(DirFwd, INPUT); 
	pinMode(DirRev, INPUT); 
	pinMode(AIrFar, INPUT); 
	pinMode(AIrClose, INPUT); 
	pinMode(EncFwdL, INPUT); 
	pinMode(EncFwdR, INPUT); 
	pinMode(EncRearL, INPUT); 
	pinMode(EncRearR, INPUT);
}

void HARDWARE::read_sensor_values() {
	// Read the two digtial IR sensors
	digi_read.FRONT = digitalRead(DirFwd);
	digi_read.REAR = digitalRead(DirRev);

	// Read analog sensors
	ir_read.IR_FAR = 0;
	ir_read.IR_CLOSE = 0;
	for (int ii = 0; ii < read_number; ii++) {
		ir_read.IR_FAR += analogRead(AIrFar);
		ir_read.IR_CLOSE += analogRead(AIrClose);
	}
	ir_read.IR_FAR = ir_read.IR_FAR / read_number;
	ir_read.IR_CLOSE = ir_read.IR_CLOSE / read_number;

	// Read all the encoder sensors and store their values  
	encoder_read.F_L = digitalRead(EncFwdL);
	encoder_read.F_R = digitalRead(EncFwdR);
	encoder_read.R_L = digitalRead(EncRearL);
	encoder_read.R_R = digitalRead(EncRearR);
}

int HARDWARE::get_analog_reading(int sensor) {
	if (sensor == 1) {
		return ir_read.IR_FAR;
	}
	if (sensor == 2) {
		return ir_read.IR_CLOSE;
	}
}

int HARDWARE::get_digital_reading(int sensor) {
	if (sensor == 1) {
		return digi_read.FRONT;
	}
	if (sensor == 2) {
		return digi_read.REAR;
	}
}

int HARDWARE::get_encoder_result(int sensor) {
	if (sensor == 1) {
		return encoder_read.F_R;
	}
	if (sensor == 2) {
		return encoder_read.F_L;
	}
	if (sensor == 3) {
		return encoder_read.R_R;
	}
	if (sensor == 4) {
		return encoder_read.R_L;
	}
}

int IR_CALCULATION::get_distance(HARDWARE& sensor_data, int sensor) {
	double cal_value; 
	cal_value = sensor_data.get_analog_reading(sensor) 
		* (sensor_data.system_voltage / 1024); 
  
	if (sensor == 1) {
		// The distance is calcualted from the GP2Y0A710K0F data sheet. Power function with
		// R^2 == .973 
		return round((1199.55 * pow(cal_value, -2.833)));
	}
	if (sensor == 2) {
		// The distance is calculated using data points from the GP2Y0A02YK data sheet
		// This equation relates the distance vs the voltage reading. This is a power function with 
		// an R^2 value == .9961
		return round((62.1418 * pow(cal_value, -1.115)));
	}
}

bool IR_CALCULATION::ObjectImmediatelyClose(HARDWARE& sensor_data, int sensor) {
	int val;
	val = sensor_data.get_digital_reading(sensor);
	if (val == 1) {
		return false;
	}
	if (val == 0) {
		return true;
	}
}

bool OBJECT_DETECTION::SensorOverLapExists() {
	// The sensors overlap between 100 cm and 120. 
	// If both readings are between these, we assume that there is a sensor overlap 
	// If close reports less than 70, there is no overlap, if far returns larger than 120 
	// there is no overlap. If we itterate through all the readings and we did not return false, 
	// we assume there is a sensor overlap 

	for (int ii = 0; ii < read_number; ii++) {
		if (close_read[ii] < far_lower_bound) {
			return false;
		}
		if (far_read[ii] > close_upper_bound) {
			return false;
		}
	}
	return true;
}

bool OBJECT_DETECTION::ObjectInBlindSpot(HARDWARE& sensor_data, IR_CALCULATION& sensor_result) {
	// If we read values approaching the lower limit of the close range sensor, we 
	// inform the robot that there might be an object out of the sensor range and that it 
	// should proceed with caution. This check only occurs if the values are decreasing
	// towards the lower bound AND there was an object detected in front of the robot already 

	if (!ObjectInCloseSensorRange) {
		Serial.print(F("No object in blind spot.\n"));
		return false;
	}

	// Perform sample analysis 
	close_analysis.array_evaluation(close_read, read_number, close_lower_bound);

	// Check if values are approaching the the close range sensor's blind spot. This is skipped if we already determined
	// an object to be within the blind spot 
	if (!ObjectMayBeInBlindspot && close_analysis.IsArrayDecreasingToValue() &&
		ObjectInCloseSensorRange) {
		ObjectMayBeInBlindspot = true;
		Serial.print(F("Object detected possibly entering blind spot.\n Requesting Cautionary "
			"speed.\n"));
		return true;
	}
	// Clears object in blind spot caution. Assuming we already determined an object to be in our blind spot. 
	// only returns false if the object clears the blind spot and no object appears 
	// This check clears only if the IR sensor has increased in value to a specified amount and the collisoin check 
	// has not fired. If the collision check does fire, we assume an object is in the blind spot 
	if (ObjectMayBeInBlindspot) {
		close_analysis.array_evaluation(close_read, read_number, close_blind_spot_limit);

		if (close_analysis.IsArrayIncreasingToValue() &&
			!sensor_result.ObjectImmediatelyClose(sensor_data, front)) {
			ObjectMayBeInBlindspot = false;
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

// TODO: Test implemented methods. Overlap, blind spot logic, most accurate reading and 
// estimated object maneuver. 
int OBJECT_DETECTION::closest_object_distance(HARDWARE& sensor_data, 
	IR_CALCULATION& sensor_result) {

	// Read values from both sensors
	int last_element = read_number - 1;
	for (int ii = 0; ii < read_number; ii++) {
		far_read[ii] = sensor_result.get_distance(sensor_data,far);
		close_read[ii] = sensor_result.get_distance(sensor_data, close);
	}

	// Check for blind spots. This check only occurs if an object was previously detected
	// by the close range sensor. This returns FALSE if there was never an object detected
	// or if the blind spot check was cleared. 
	if (ObjectInBlindSpot(sensor_data, sensor_result)) {
		return close_read[last_element];
	}
	Serial.print("No object in blind spot\n");

	// Array analysis 
	close_analysis.array_evaluation(close_read, read_number, close_upper_bound);
	far_analysis.array_evaluation(far_read, read_number, far_lower_bound);

	// Account for object transition between the sensors. IE, if the object leaves the close
	// detection range or enters the far sensor detection range 
	if (SensorOverLapExists()) {
		Serial.print("Sensor Overlap detected.\n");
		// Object moving from close range sensor to far range sensor 
		// Array is increasing towards close upper bound (close range sensor) 
		if (close_analysis.IsArrayIncreasingToValue()) {

			ObjectInCloseSensorRange = false;
			ObjectInFarSensorRange = true;
			Serial.print(F("Object has moved away from robot\n"));
			return far_read[last_element];
		}
		// Object moving from far range sensor to close range 
		if (far_analysis.IsArrayDecreasingToValue()) {

			ObjectInCloseSensorRange = true;
			ObjectInFarSensorRange = false;
			Serial.print(F("Object has moved toward robot\n"));
			return close_read[last_element];
		}
		// Object is stationary between the two sensors 
		else {
			ObjectInCloseSensorRange = true;
			ObjectInFarSensorRange = false;
			Serial.print(F("Object is predicted to be stationary.\n"));
			return close_read[last_element];
		}
	}

	// Report the most accurate sensor reading. 
	// Both sensors report can report 70cm at the same time, so we trust the close range
	// sensor if it reports a value less than 120. If we get a higher value then we trust the
	// far range sensor 
	else {
		Serial.print("No overlap\n");

		// Check to see if the close range sensor value is less than the upper bound 
		// if it is, we trust that reading. If the distance is greater than 120 on the close range,
		// we return the far range distance instead
		if (close_analysis.IsLowerThanValue()) {
			Serial.print(F("Object is in close sensor range!\n"));
			ObjectInCloseSensorRange = true;
			ObjectInFarSensorRange = false;
			return close_read[last_element];
		}
		else {
			Serial.print(F("Object is in far sensor range!\n"));
			ObjectInCloseSensorRange = false;
			ObjectInFarSensorRange = true;
			return far_read[last_element];
		}

	}
}

double OBJECT_DETECTION::detected_object_speed(unsigned long Time) {
	// Measure the difference in position from the first reading to the last reading, 
	// to make the calculation faster, we only run it on the the sensor which has the object 
	// in its current range 
	int displacement;
	// TODO: Test to determine the best buffer value. Used to determine if an object is static
	int buffer = .8;

	if (ObjectInCloseSensorRange) {
		displacement = close_analysis.total_array_difference();
	}
	if (ObjectInFarSensorRange) {
		displacement = far_analysis.total_array_difference();
	}

	// Current object status is determined by its displacement 
	// If dispacement is negative, the object is moving away
	// Divide by 100 as the displacement is in cm 
	return ((displacement / 100) / Time);
}

bool OBJECT_DETECTION::CautionRequiredDueToObject() {
	// Retruns true if any cautionary value is true
	if (ObjectMayBeInBlindspot) {
		return true;
	}
}