//
// Created by henry on 1/7/18.
//

#ifndef AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H
#define AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H


#include <Arduino.h>
#include "HardwareProperties.h"

namespace hardware {

/** This will be the base class that the Analog and Digital sensors will extend
 *  off of. It will read voltaget from the pin that it is attached to with the
 *  read  method. */
class AbstractSensor : Pluggable {
protected:
    /** This is the pin number that this sensor will read data from. */
    uint8_t pin = NONE;

public:
    /** This will return the value read from PIN. */
    virtual int read()=0;

    /** Associate this IR sensor with pin INPUT. Returns true iff the pin is
     *  a valid pin*/
    boolean attach(HW_pins input) override;
};

class AnalogSensor : AbstractSensor {
    int read() override;
};

class DigiSensor : AbstractSensor {
    int read() override;
};

} // HARDWARE
#endif //AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H
