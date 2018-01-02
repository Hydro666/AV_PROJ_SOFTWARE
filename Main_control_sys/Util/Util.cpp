/*
 Name:		Util.cpp
 Created:	12/28/2017 3:06:08 PM
 Author:	Aquiles Gomez
*/

#include "Util.h"

// Returns TRUE if the values are generally decreasing in the array 
// We compare the number of increases over the number of decreases
// if the increases are more than the decreases then we return False
bool ArrayIsDecreasing(double arr[], int n) {
	// Number of increases 
	int increases = 0; 
	// Number of decreases 
	int decreases = 0; 

	// Itterate over the array, we do not check the first element as we begin there
	for (int i = 1; i < n; i++) {
		// Check to see if the next value is larger than the previous 
		if (arr[i] > arr[i - 1]) {
			increases++;
		}
		// Check to see if the next value is larger than the previous 
		if (arr[i] < arr[i - 1]) {
			decreases++;
		}
	}

	// If we have more decreases than increases return true, otherwise
	// return false. 
	if (decreases > increases) {
		return true;
	}
	else {
		return false;
	}
}
