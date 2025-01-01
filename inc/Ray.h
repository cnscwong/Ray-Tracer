#pragma once
#include "Tuple.h"

// Class for rays and ray operations
class Ray{
private:
    // Stores the coordinates the ray originates from
    Point origin = Point(0, 0, 0);
    // Stores the direction and speed the ray travels in one time unit
    Vector direction = Vector(1, 1, 1);
public:
    // Ray constructor
    Ray(Point o, Vector d);

    // Getters
    Point getOrigin();
    Vector getDirection();

    // Computes the position of the ray at time t
    Tuple computePosition(float t);
};
