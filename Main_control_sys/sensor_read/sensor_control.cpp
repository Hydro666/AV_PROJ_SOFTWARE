/*
 Name:		sensor_control.cpp
 Created:	11/14/2017 9:18:13 PM
 Author:	Aquiles Gomez, Henry Lancelle 
*/

#include "sensor_control.h"
#include <AbstractSensor.h>

//IR_SENSOR::IR_SENSOR(uint8_t readFrom) : AbstractSensor(readFrom, analogRead){}

void IR_SENSOR::ir_begin(int pin, double volt) {
	sensor = pin; 
	volt_convert = volt / 1024; 
	readings = 5; 
	pinMode(sensor, INPUT); 
}

// TODO: Need to improve on how this is implemented. It currently limits the sensor readings 
// so it messes with the object detection. 
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

int IR_SENSOR::read_close() {
	double cumulative_value = 0;  
	double cal_value;

	// Obtain the input over a number of readings 
	for (int i = 0; i < readings; i++) {
		cumulative_value += analogRead(sensor);
	}

	// The distance is calculated using data points from the GP2Y0A02YK data sheet
	// This equation relates the distance vs the voltage reading. This is a power function with 
	// an R^2 value == .9961
	// We scale the values we read between 65  and 372 as these are the absolute minimums 
	// (per datasheet), utilizing the calibrated values to account for ambient lighting 
	cal_value = (cumulative_value / readings) * volt_convert;

	return round((62.1418 * pow(cal_value, -1.115)));
}

int IR_SENSOR::read_far() {
	double cumulative_value = 0; 
	double cal_value;

	// Obtain the readings 
	for (int i = 0; i < readings; i++) {
		cumulative_value += analogRead(sensor);
	}

	// The distance is calcualted from the GP2Y0A710K0F data sheet. Power function with
	// R^2 == .973 Readings are scaled between 211 and 454 as these are the absolute 
	// minimums and maximums respectively (datasheet) utilizing the calibrated values 
	// to account for ambient lighting 
	cal_value = (cumulative_value/readings) * volt_convert;

	return round((1199.55 * pow(cal_value, -2.833)));
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

void ENCODER::read_value() {
	value = digitalRead(sensor);
	if (value == 0) {
		tick++;
	}
}

void ENCODER::encoder_begin(int pin) {
	sensor = pin;
	pinMode(sensor, INPUT);
	wheel_circumference = .20420352248;
	tick = 0;
	wheel_rotations = 0;
	dis_traveled = 0;
}

void ENCODER::begin_speed_calc() {
	timeStart = millis();
}


void ENCODER::end_speed_calc() {
	timeEnd = millis();
	time = (timeEnd - timeStart) / 1000;
	wheel_rotations += (tick / 20);
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

void ENCODER::reset() {
	tick = 0;
	wheel_rotations = 0;
	dis_traveled = 0;
}


void OBJECT_DETECTION::configure_object_detection(int volt) {
	Serial.print(F("Configuring object detection controls.\n"));
	far.ir_begin(13, volt);
	close.ir_begin(14, volt);
	fwd.digi_begin(44);
	bck.digi_begin(45);
	ObjectInCloseSensorRange = false;
	ObjectInFarSensorRange = false;

	Serial.print(F("Object Detection configuration completed.\n"));
}

bool OBJECT_DETECTION::SensorOverLapExists() {
	// The sensors overlap between 100 cm and 120. 
	// If both readings are between these, we assume that there is a sensor overlap 
	// If close reports less than 70, there is no overlap, if far returns larger than 120 
	// there is no overlap. If we itterate through all the readings and we did not return false, 
	// we assume there is a sensor overlap 

	for (int i = 0; i < read_number; i++) {
		if (close_read[i] < far_lower_bound) {
			return false; 
		}
		if (far_read[i] > close_upper_bound) {
			return false; 
		}
	}
	return true; 
}

bool OBJECT_DETECTION::ObjectInBlindSpot() {
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
			!fwd.ObjectIsToClose()) {
			ObjectMayBeInBlindspot = false; 
			return false;
		}
		else {
			return true; 
		}
	}
	else{
		return false; 
	}
}

// TODO: Test implemented methods. Overlap, blind spot logic, most accurate reading and 
// estimated object maneuver. 
int OBJECT_DETECTION::closest_object_distance() {
	int last_element = read_number - 1;

	// Read values from both sensors
	for (int i = 0; i < read_number; i++) {
		far_read[i] = far.read_far();
		close_read[i] = close.read_close();
	}

	// Check for blind spots. This check only occurs if an object was previously detected
	// by the close range sensor. This returns FALSE if there was never an object detected
	// or if the blind spot check was cleared. 
	if (ObjectInBlindSpot()) {
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
	if (displacement < -buffer) {
		ObjectIsMovingAway = true; 
		ObjectIsLikelyStatic = false; 
	}
	if (displacement > buffer) {
		ObjectIsMovingTowards = true; 
		ObjectIsLikelyStatic = false;
	}
	else {
		ObjectIsMovingAway = false; 
		ObjectIsMovingTowards = false; 
		ObjectIsLikelyStatic = true; 
	}

	// Divide by 100 as the displacement is in cm 
	return ((displacement / 100) / Time); 
}

bool OBJECT_DETECTION::ObjectApproaching() {
	return ObjectIsMovingTowards;
}

bool OBJECT_DETECTION::ObjectLeaving() {
	return ObjectIsMovingAway;
}

bool OBJECT_DETECTION::ObjectStatic() {
	return ObjectIsLikelyStatic;
}

bool OBJECT_DETECTION::CautionRequiredDueToObject() {
	// Retruns true if any cautionary value is true
	if (ObjectMayBeInBlindspot) {
		return true;
	}
}
