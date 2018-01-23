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
} // HARDWARE