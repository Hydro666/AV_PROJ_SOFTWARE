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

int AnalogSensor::read() {
    return analogRead(pin);
}

int DigiSensor::read() {
    return digitalRead(pin);
}
} // HARDWARE


