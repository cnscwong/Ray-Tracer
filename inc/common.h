#pragma once
#include <cmath>
#include <vector>

// Threshold to check float equality |a - b| < EPSILON
const float EPSILON = 0.0001f;
bool floatIsEqual(float a, float b);

// Function to count the number of digits of an int
int countDigits(int n);