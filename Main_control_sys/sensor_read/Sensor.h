//
// Created by henry on 1/7/18.
//

#ifndef AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H
#define AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H


#include <Arduino.h>
#include "HardwareProperties.h"

namespace hardware {

/** This will be the base class that the Analog and Digital sensors will extend
 *  off of. It will handle reading data and writing it to a member array. */
class AbstractSensor {
protected:
    /** This is the pin number that this sensor will read data from. */
    uint8_t pin;

public:
    /** This will return the value read from PIN. */
    virtual int read()=0;

    /** Returns the average voltage of NUM measurments taken every DELAY
     *  milliseconds.*/
    float averageRead(short int num, short int delay);

    /** Associate this IR sensor with pin INPUT. Returns true iff the pin is
     *  a valid pin*/
    boolean attach(HW_pins input);
};

class IR_sensor : AbstractSensor {
    int read() override;
};

class Digi_sensor : AbstractSensor {
    int read() override;
};

} // HARDWARE
#endif //AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H
