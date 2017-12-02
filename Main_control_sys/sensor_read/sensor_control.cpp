/*
 Name:		sensor_read.cpp
 Created:	11/14/2017 9:18:13 PM
 Author:	Aquiles Gomez
*/

#include "sensor_control.h"

// Convert constant used to convert IR readings back to voltage. Obtained by dividing the 
// input voltage (7V) by the steps avaiable (1024). 
double vol_convert = .0068359375; 

// Pins 
int far = A8; 
int close = A9; 

// IR sensor calibration values 
double c1_c = 0.00;
double c2_c = 3.64;
double c1_f = 1.97;
double c2_f = 3.49;

// Sets up the pins values and sets them as inputs 
int ir_pin_setup() {
	pinMode(far, INPUT);
	pinMode(close, INPUT);
}

// Check to see if there are previously calibrated values. Returns TRUE if there are
// previously calibrated values. 
bool is_previously_calibrated(double c1_c, double c2_c, double c1_f, double c2_f)
{
	// Addresses to check. We only check the first 4 values within the EEPROM as there are only
	// four calibration values. 
	int eeAddress = 0;
	EEPROM.get(eeAddress, c1_c);
	eeAddress += sizeof(double);
	EEPROM.get(eeAddress, c2_c);
	eeAddress += sizeof(double);
	EEPROM.get(eeAddress, c1_f);
	eeAddress += sizeof(double);
	EEPROM.get(eeAddress, c2_f);

	// Check to see if we have digits read and if they are greater than zero. We only check if 
	// the first and last value as if they are digits, we assume the ones in between are as well
	// The minimum value of second sensor cannot be zero, thus we assume that if it zero it has not been 
	// calibrated before

	if ((c1_c >= 0.00) && (c2_f != 0.00)) {
		return true;
	}
	return false; 
}

// Reads the assigned values and prints them out to the serial monitor 
double print_values(double c1_c, double c2_c, double c1_f, double c2_f) {
	Serial.print("These are the calibrated values!");
	Serial.print("\nThe values are: \n");
	Serial.print("c1_c: "); Serial.print(c1_c);
	Serial.println();
	Serial.print("c2_c: "); Serial.print(c2_c);
	Serial.println();
	Serial.print("c1_f: "); Serial.print(c1_f);
	Serial.println();
	Serial.print("c2_f: "); Serial.print(c2_f);
	Serial.println();
}

// Writes the variable inputs to the Arduino's permanent memory 
void write_to_memory(double c1_c, double c2_c, double c1_f, double c2_f) {
	int eeAdress = 0; 
	EEPROM.put(eeAdress, c1_c);
	eeAdress += sizeof(double);
	EEPROM.put(eeAdress, c2_c);
	eeAdress += sizeof(double);
	EEPROM.put(eeAdress, c1_f);
	eeAdress += sizeof(double);
	EEPROM.put(eeAdress, c1_f);
	eeAdress += sizeof(double);
}

// Delays for x seconds, printing the seconds remaining
void delay_x_secs(int x) {
	for (int i = x; i != 0; i--) {
		Serial.print(i); 
		Serial.print(" seconds remaining...");
		Serial.println();
		delay(1000);
	}
	Serial.print("Ready for reading!\n");
}

// Reads the values for calibration 
double get_cal_val(int pin, int duration) {
	double cumulativeValue = 0;
	int read_number = 0;
	unsigned long timeStart = millis();
	double result; 
	int x = 10;

	delay_x_secs(x);
	delay(duration);
	do {
		delay(20);
		cumulativeValue += analogRead(pin);
		read_number++;
	} while (millis() <= timeStart + duration);
	result = cumulativeValue / read_number; 
	return result * vol_convert; 
}

// Function that calibrates for the ambient light in the room. This is only run during 
// the first time setup within a new environment. This function assigns the calibrated 
// values to each variable input for the respective sensor
float calibrate(double& c1_c, double& c2_c, double& c1_f, double& c2_f, 
	int close, int far) {
	// Read values from the sensor for a 'long time' to get more consistent readings
	// averaging out the readings to get a consistent value 
	int duration = 1000;

	// Get calibration value for the close range sensor (object far away from sensor) 
	Serial.print("Calibration for the close range sensor! Place an item far from the sensor"
		"(~150 cm away)");
	c1_c = get_cal_val(close, duration);

	// Get calibration value for the close range sensor (object near sensor) 
	Serial.print("Calibration for close range sensor! Place an item near the sensor! (~20 cm away)\n");
	c2_c = get_cal_val(close, duration);

	// Get calibration value for the long range sensor (object far from sensor)
	Serial.print("Calibration for the long range sensor! Place an item far from the sensor! (~550 cm away)\n");
	c1_f = get_cal_val(far, duration);

	// Get calibration value for the long range sensor (object near sensor) 
	Serial.print("Calibration for the long range sensor! Place an item near the sensor! (~100 cm away)\n");
	c2_f = get_cal_val(far, duration);

	// Check to see if any of the values of the second sensor
	// are zero as this means there must have been an error 
	if ((c1_f == 0) || (c2_f == 0)) {
		Serial.print("Error!"); 
	}
	write_to_memory(c1_c, c2_c, c1_f, c2_f);
	print_values(c1_c, c2_c, c1_f, c2_f);
}

// Function that obtains the approximate distance from the close range sensor.
// We average out from a number of readings to obtain a more consistent result. 
// The number of readings can be tweaked to increase performance or to make 
// the reading more accurate. Returns current distance value from the close range sensor 
double read_distance_close(int readings) {
	double cumulative_value = 0; 
	double volt_read; 
	double current_distance; 

	// Obtain the readings
	for (int i = 0; i < readings; i++) {
		delay(20);
		cumulative_value += analogRead(close);
	}

	// The distance is calculated using data points from the GP2Y0A02YK data sheet
	// This equation relates the distance vs the voltage reading. This is a power function with 
	// an R^2 value == .9961
	// We scale the values we read between .2 V and 3 V as these are the absolute minimums 
	// (per datasheet), utilizing the calibrated values to account for ambient lighting 
	volt_read = map((cumulative_value / readings)*vol_convert , c1_c, c2_c, 0.2, 3.0); 
	current_distance = 62.1418 * pow(volt_read, -1.115);

	return current_distance; 
}

// Function that obtains the approximate distance of the long range sensor. 
// We average our readings to obtain a more consistent result. The number 
// of readings can be tweaked to increase performance or to make the 
// readings more accurate. Returns current distance value from long range sensor
double read_distance_far(int readings) {
	double cumulative_value = 0; 
	double volt_read; 
	double current_distance; 

	// Obtain the readings 
	for (int i = 0; i < readings; i++) {
		delay(20);
		cumulative_value += analogRead(far);
	}

	// The distance is calcualted from the GP2Y0A710K0F data sheet. Power function with
	// R^2 == .973 Readings are scaled between 1V and 3.5V as these are the absolute 
	// minimums and maximums respectively (datasheet) utilizing the calibrated values 
	// to account for ambient lighting 
	volt_read = map((cumulative_value / readings) * vol_convert, c1_f, c2_f, 1.0, 3.5); 
	current_distance = 1199.55 * pow(volt_read, -2.833); 

	return current_distance;
	
}

