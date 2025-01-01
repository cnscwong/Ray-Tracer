#include "Colour.h"
#include "common.h"

// Colour constructors

// Default colour constructor to black
Colour::Colour(){
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

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
// Returns the colour this + a
Colour Colour::operator+(Colour a){
    return Colour(r + a.r, g + a.g, b + a.b);
}

// Returns the colour this - a
Colour Colour::operator-(Colour a){
    return Colour(r - a.r, g - a.g, b - a.b);
}

// Returns the colour this*scale
Colour Colour::operator*(float scale){
    return Colour(r*scale, g*scale, b*scale);
}

// Hadamard product(this * a). Conceptually the combination of viewing one colour under
// a coloured light
Colour Colour::operator*(Colour a){
    return Colour(r*a.r, g*a.g, b*a.b);
}