/*
Name:		Util.h
Created:	12/28/2017 3:06:08 PM
Author:	Aquiles Gomez, Henry Lancelle
This houses utility functions that can be multipurposed. Typically used
by other libraries
*/

#ifndef _Util_h
#define _Util_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else

#include <queue>
#include <stdexcept>
#include "WProgram.h"
#endif

class ARRAY {
private:
	// Values used to evaluate arrays 
	int increases;
	int decreases;
	int moving_towards;
	int moving_from;
	int buffer_1;
	int buffer_2;


	// Accessed by public functions to determine an array's properties 
	bool ArrayIsDecreasingToValue;
	bool ArrayIsIncreasingToValue;
	bool GreaterThanValue;
	bool LowerThanValue;

	int array_difference;
public:
	// Takes an array as an input and itterates over the values of the input
	// array
	void array_evaluation(int[], int, int);

	// TODO: Add booleans that return the specified inputs found below
	// I.E Array is increasesing to value... etc. 
	bool IsArrayIncreasingToValue();
  
	bool IsArrayDecreasingToValue();

	bool IsGreaterThanValue();

	bool IsLowerThanValue();

	int total_array_difference();
};

/** This is a container that can contain at most SIZE number of items.  If the
 *  maximum capacity has been reached, then the next element added to this
 *  container will remove the oldest element.*/
template <class T>
class FiniteQueue : std::deque {
private:
    int maxSize;

public:
    FiniteQueue(int size) {
        maxSize = size;
    }

    void add(T item) {
        if (size() > maxSize) {
            throw std::runtime_error("The size of the queue is larger than the maximum allowable!");
        } else if (size() == maxSize) {
            pop_back();
        }
        push_front(item);
    }

    int getMaxSize() {
        return maxSize;
    }
};

#endif