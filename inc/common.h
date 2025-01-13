#pragma once
#include <cmath>
#include <vector>

const float PI = 3.14159265358979323846f;

// Threshold to check float equality |a - b| < EPSILON
const float EPSILON = 0.0015f;
bool floatIsEqual(float a, float b);

// Function to count the number of digits of an int
int countDigits(int n);