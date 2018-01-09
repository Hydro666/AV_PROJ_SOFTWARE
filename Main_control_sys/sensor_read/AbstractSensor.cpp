//
// Created by henry on 1/7/18.
//

#include "AbstractSensor.h"

AbstractSensor::AbstractSensor(uint8_t pin, int (*func)(uint8_t)) {
    this->pin = pin;
    this->reader = func;
}

int AbstractSensor::read() {
    return reader(pin);
}

void AbstractSensor::attach(uint8_t input) {
    if ()
}

float AbstractSensor::averageRead(short int num, short int offset) {
    int total = 0;
    for (short int i = num; i > 0; --i) {
        delay(offset);
        total += read();
    }
    return (total + 0.0) / num; // The addend of 0.0 is so that true division will be performed and not truncated division.
}
