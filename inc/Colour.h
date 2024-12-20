#pragma once
#include <string>

// Class to store colour properties
class Colour{
public:
    float r, g, b;
    Colour();
    Colour(float r, float g, float b);
    bool isEqual(Colour a);
};

// Colour operations
Colour addColours(Colour a, Colour b);
Colour subtractColours(Colour a, Colour b);
Colour scaleColour(Colour a, float scale);
Colour multiplyColour(Colour a, Colour b);