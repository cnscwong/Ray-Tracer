#include <cmath>
#include "common.h"

// Checks if the difference between 2 floats is under a threshold value
bool floatIsEqual(float a, float b){
    if(abs(a - b) < EPSILON){
        return true;
    }
    return false;
}

// Function to count digits
int countDigits(int n){
      // Base case
    if (n == 0)
        return 1;
  
    int count = 0;
  
      // Iterate till n has digits remaining
    while (n != 0) {
      
          // Remove rightmost digit
        n = n / 10;
      
          // Increment digit count by 1
        ++count;
    }
    return count;
}