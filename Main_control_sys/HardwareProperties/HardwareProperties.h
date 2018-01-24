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

/** Each pin name corresponds to the physical pin number that the hardware
 *  component is plugged into. */
enum HW_pins : uint8_t {
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

/** An type corresponding to eight directions similar to a compass.*/
enum CardinalDirection {
    front,
    left,
    right,
    rear,
    frontLeft,
    frontRight,
    rearLeft,
    rearRight
};

/** Type that enumerates the distances CLOSE and FAR.*/
enum Distance {
    close,
    far
};

/** This interface will be implemented by every hardware component that can be
 *  plugged into some pin in the arduino board.*/
class Pluggable {
    virtual boolean attach(HW_pins pin)=0;
};

// Starts all the hardware
void Hardware_begin(double voltage);

} // HARDWARE

#endif //AV_PROJ_SOFTWARE_HARDWAREPROPERTIES_H
