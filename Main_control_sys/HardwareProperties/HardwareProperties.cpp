/*
 Name:		HardwareProperties.cpp
 Created:	1/18/2018 4:34:29 PM
 Author:	aquil
 Editor:	http://www.visualmicro.com
*/

#include "HardwareProperties.h"

namespace hardware {
boolean validPin(HW_pins pin) {
    switch (pin) {
        case DirFwd: case DirRev: case AIrFar: case AIrClose:
        case EncFwdL: case EncFwdR: case EncRearL: case EncRearR:
            return true;
        default:
            return false;
    }
}

void Hardware_begin() {
    // Set as inputs
    pinMode(DirFwd, INPUT);
    pinMode(DirRev, INPUT);
    pinMode(AIrFar, INPUT);
    pinMode(AIrClose, INPUT);
    pinMode(EncFwdL, INPUT);
    pinMode(EncFwdR, INPUT);
    pinMode(EncRearL, INPUT);
    pinMode(EncRearR, INPUT);
}

} // HARDWARE