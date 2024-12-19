#include <cmath>
#include "common.h"

// Checks if the difference between 2 floats is under a threshold value
bool floatIsEqual(float a, float b){
    if(abs(a - b) < EPSILON){
        return true;
    }
    return false;
}