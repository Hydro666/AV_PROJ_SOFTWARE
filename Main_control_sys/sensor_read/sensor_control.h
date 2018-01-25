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

#include <math.h>
#include <deque>
#include <map>

#include "HardwareProperties.h"
#include "Util.h"
#include "Sensor.h"

namespace hardware {

/** This class will control reading and timing of reads from the pluggable
 *  hardware components. Some reads will be stored, and when the values of
 *  reads is requested by a client, the average of the stored reads will be
 *  sent to the client class.*/
class SensorControl {
    /* Storage of values will be implemented by deques. This way we can only
     * store values up to a certain size and then easily discard the oldest
     * value currently stored.*/

    /** Storage of reads from the AnalogSensors.*/
    struct analog_values {
        std::deque<int> *irFar;
        std::deque<int> *IR_CLOSE;
    } irRead;
    struct digital_values {
        std::deque<int> *FRONT;
        std::deque<int> *REAR;
    } digiRead;
    //Encoder values
    struct encoder_values {
        std::deque<int> *F_R;
        std::deque<int> *F_L;
        std::deque<int> *R_R;
        std::deque<int> *R_L;
    } encoderRead;

    // Returns analog reading, 1 for far, 2 for close
    int get_analog_reading(int sensor);

    // Returns digital reading 1 for front, 2 for rear
    int get_digital_reading(int sensor);

    // Returns encoder reading 1 for front right, 2 for front left,
    // 3 for rear right, 4 for rear left
    int get_encoder_result(int sensor);

    // Reads all sensor values and notes time of last reading
    void read_sensor_values();


    double system_voltage;

    /** The maximum number of stored values for each individual sensor plugged
     *  into the arduino board. */
    const int maxStorage = 5;
};

} // Namespace HARDWARE

class IR_CALCULATION;
// Interprets the results from ir sensor in the HARDWARE class
class IR_CALCULATION {
public:
    IR_CALCULATION(hardware::AnalogSensor& left, hardware::AnalogSensor& right);
private:
    /** These will be the sensors that correspond to the left and right IR
     *  sensor. */
    hardware::AnalogSensor *leftSensor;
    hardware::AnalogSensor *rightSensor;
    std::map<hardware::CardinalDirection, hardware::Encoder> encoders;

    // Returns the calculated distance from the specified sensor
    // 1 for far, 2 for close
    int get_distance(HARDWARE &sensor_data, int sensor);

    // Returns true if either the front sensor or rear sensor reports an object
    bool ObjectImmediatelyClose(HARDWARE &sensor_data, int sensor);
};

class OBJECT_DETECTION;
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