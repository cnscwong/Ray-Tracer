#include "Ray.h"

// Ray constructors
Ray::Ray(){
    origin = Point();
    direction = Vector();
}

Ray::Ray(Point o, Vector d){
    origin = o;
    direction = d;
}

// Returns private variables origin and direction
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

// Transforms the ray by the matrix m
Ray Ray::transform(Matrix m){
    return Ray(Point(m*origin), Vector(m*direction));
}