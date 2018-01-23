//
// Created by henry on 1/7/18.
//

#ifndef _HardwareProperties_h
#define _HardwareProperties_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>

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

namespace hardware {
enum HW_pins : int {
    DirFwd = 44,
    DirRev = 45,
    AIrFar = 13,
    AIrClose = 14,
    EncFwdL = 30,
    EncFwdR = 31,
    EncRearL = 32,
    EncRearR = 33,
    NONE = -1
};

/** Return true iff pin is a valid pin that can be selected.*/
boolean validPin(uint8_t pin);

enum sensor_return : int {
    far = 1,
    close = 2,
    front = 1,
    rear = 2,
    front_right = 1,
    front_left = 2,
    rear_right = 3,
    rear_left = 4
};
} // HARDWARE

#endif //AV_PROJ_SOFTWARE_HARDWAREPROPERTIES_H
