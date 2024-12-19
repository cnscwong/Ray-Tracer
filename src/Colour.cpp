#include "Colour.h"
#include "common.h"

// Colour constructor
Colour::Colour(float r, float g, float b){
    this->r = r;
    this->g = g;
    this->b = b;
}

// Checks if two colours are equal
bool Colour::isEqual(Colour a){
    if(!floatIsEqual(r, a.r) || !floatIsEqual(g, a.g) || !floatIsEqual(b, a.b)){
        return false;
    }
    return true;
}

// Colour operations, works the same as tuple operations

// Returns the colour a + b
Colour addColours(Colour a, Colour b){
    return Colour(a.r+b.r, a.g+b.g, a.b+b.b);
}

// Returns the colour a - b
Colour subtractColours(Colour a, Colour b){
    return Colour(a.r-b.r, a.g-b.g, a.b-b.b);
}

// Returns the colour a*scale
Colour scaleColour(Colour a, float scale){
    return Colour(a.r*scale, a.g*scale, a.b*scale);
}

// Hadamard product. Conceptually the combination of viewing one colour under
// a coloured light
Colour multiplyColour(Colour a, Colour b){
    return Colour(a.r*b.r, a.g*b.g, a.b*b.b);
}