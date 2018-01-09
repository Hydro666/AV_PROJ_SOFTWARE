//
// Created by henry on 1/7/18.
//

#ifndef AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H
#define AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H


#include <Arduino.h>

/** This will be the base class that the Analog and Digital sensors will extend
 *  off of. It will handle reading data and writing it to a member array. */
class AbstractSensor {
private:
    /** This is the pin number that this sensor will read data from. */
    uint8_t pin;

    /** This is the function that will read from PIN when called. */
    int (*reader)(uint8_t);

public:
    AbstractSensor(uint8_t pin, int (*func)(uint8_t) );
    /** This will return the value read from PIN. */
    virtual int read();

    /** Returns the average voltage of NUM measurments taken every DELAY
     *  milliseconds.*/
    float averageRead(short int num, short int delay);

    /** Associate this IR sensor with pin INPUT. */
    void attach(uint8_t input);
};


#endif //AV_PROJ_SOFTWARE_ABSTRACTSENSOR_H
