/*
 Name:		Util.cpp
 Created:	12/28/2017 3:06:08 PM
 Author:	Aquiles Gomez, Henry Lancelle
*/

#include "Util.h"

void ARRAY::array_evaluation(int array[], int array_size, int value) {
	// Initally define the values 
	increases = 0;
	decreases = 0;
	moving_towards = 0;
	moving_from = 0;
	array_difference = 0;
	buffer_1 = value + 3;
	buffer_2 = value - 3;

	// Itterate over the array and compare the number of decreases and increases 
	for (int i = 1; i < array_size; i++) {
		// Check to see if the values are larger or smaller than the previous value 
		if (array[i] > array[i - 1]) {
			increases++;
		}
		if (array[i] < array[i - 1]) {
			decreases++;
		}
		if (array[i] > value) {
			GreaterThanValue = true;
			LowerThanValue = false;
		}
		if (array[i] < value) {
			LowerThanValue = true;
			GreaterThanValue = false;
		}
		if (((array[i] < (buffer_1)) && (array[i] >= value)) ||
			((array[i] > (buffer_2)) && (array[i] <= value))) {
			moving_towards++;
		}
		else {
			moving_from++;
			GreaterThanValue = false;
			LowerThanValue = false;
		}

		array_difference += (array[i - 1] - array[i]);
	}

	// Check to see if array is approaching value 
	if ((decreases > increases) && (moving_towards > moving_from)) {
		ArrayIsDecreasingToValue = true;

	}
	if ((increases > decreases) && (moving_towards > moving_from)) {
		ArrayIsIncreasingToValue = true;
	}
	else {
		ArrayIsDecreasingToValue = false;
		ArrayIsIncreasingToValue = false;
	}
}

bool ARRAY::IsArrayIncreasingToValue() {
	return ArrayIsIncreasingToValue;
}

bool ARRAY::IsArrayDecreasingToValue() {
	return ArrayIsDecreasingToValue;
}

bool ARRAY::IsGreaterThanValue() {
	return GreaterThanValue;
}

bool ARRAY::IsLowerThanValue() {
	return LowerThanValue;
}

int ARRAY::total_array_difference() {
	return array_difference;
}
