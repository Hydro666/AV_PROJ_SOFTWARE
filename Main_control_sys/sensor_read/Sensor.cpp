//
// Created by henry on 1/7/18.
//

#include <stdexcept>
#include "Sensor.h"

namespace hardware {

boolean Sensor::attach(HW_pins input) {
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

int Encoder::read() {
    throw std::runtime_error("Encoder is not yet supported.");
}
} // HARDWARE


