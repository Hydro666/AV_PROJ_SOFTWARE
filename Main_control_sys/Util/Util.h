/*
 Name:		Util.h
 Created:	12/28/2017 3:06:08 PM
 Author:	Aquiles Gomez
 This houses utility functions that can be multipurposed. Typically used
 by other libraries
*/

#ifndef _Util_h
#define _Util_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "math.h"

// Takes in an array of length n and returns true if 
// the values within the array are decreasing 
bool ArrayIsDecreasing(double arr[], int n); 



#endif
