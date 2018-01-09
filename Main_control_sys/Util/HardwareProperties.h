//
// Created by henry on 1/7/18.
//

#ifndef AV_PROJ_SOFTWARE_HARDWAREPROPERTIES_H
#define AV_PROJ_SOFTWARE_HARDWAREPROPERTIES_H

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
enum HW_pins : uint8_t {
    DirFwd = 44,
    DirRev = 45,
    AIrFar = 13,
    AIrClose = 14,
    EncFwdL = 30,
    EncFwdR = 31,
    EncRearL = 32,
    EncRearR = 33
};
#endif //AV_PROJ_SOFTWARE_HARDWAREPROPERTIES_H
