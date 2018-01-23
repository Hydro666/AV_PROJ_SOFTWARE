//
// Created by henry on 1/7/18.
//

#include "Sensor.h"

namespace hardware {

boolean AbstractSensor::attach(HW_pins input) {
    if (!validPin(input)) {
        return false;
    } else {
        pin = input;
        return true;
    }
}

float AbstractSensor::averageRead(short int num, short int offset) {
    int total = 0;
    for (short int i = num; i > 0; --i) {
        delay(offset);
        total += read();
    }
    return (total + 0.0) / num; // The addend of 0.0 is so that true division will be performed and not truncated division.
}

int IR_sensor::read() {
    return analogRead(pin);
}

int Digi_sensor::read() {
    return digitalRead(pin);
}
} // HARDWARE


