#pragma once
#include <string>

// Class to store colour properties
class Colour{
public:
    // Stores the rgb values of the colour
    float r, g, b;
    Colour();
    Colour(float r, float g, float b);
    bool isEqual(Colour a);

    // Colour operations
    Colour operator+(Colour a);
    Colour operator-(Colour a);
    Colour operator*(Colour a);
    Colour operator*(float scale);
};

// Colour constants
const Colour BLACK = Colour();
const Colour WHITE = Colour(1, 1, 1);