#include "Ray.h"

Ray::Ray(Point o, Vector d){
    origin = o;
    direction = d;
}

Point Ray::getOrigin(){
    return origin;
}

Vector Ray::getDirection(){
    return direction;
}

// Computes the position of the ray at time t
Tuple Ray::computePosition(float t){
    return (origin + (direction*t));
}