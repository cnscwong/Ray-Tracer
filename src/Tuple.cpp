#include "Tuple.h"
#include "common.h"
#include <cmath>

// Tuple constructor
Tuple::Tuple(float x, float y, float z, float point){
    this->x = x;
    this->y = y;
    this->z = z;
    this->point = point;
}

// Checks if another tuple is equal to self
bool Tuple::isEqual(Tuple a){
    if(!floatIsEqual(x, a.x) || !floatIsEqual(y, a.y) || !floatIsEqual(z, a.z) || !floatIsEqual(point, a.point)){
        return false;
    }

    return true;
}

// Adds tuples together. Adding a vector and point together is equivalent to starting from that point and travelling
// the distance and direction of the vector, also notice that a point(1) + vector(0) results in another point! Adding
// two vectors results in another vector(0 + 0 = 0)! Adding two points results in 1 + 1 = 2 (invalid)
Tuple addTuples(Tuple a, Tuple b){
    return Tuple(a.x + b.x, a.y + b.y, a.z + b.z, a.point + b.point);
};

// Performs a - b. Intuitively, subtracting a point from a point generates a vector from p2 to p1. Subtracting a point
// from a vector moves the point back the vector's distance and direction. Subtracting two vectors represents the change
// in direction between the two.
Tuple subtractTuples(Tuple a, Tuple b){
    return Tuple(a.x - b.x, a.y - b.y, a.z - b.z, a.point - b.point);
}

// Negates a tuple. Flipping direction of a vector
Tuple negateTuple(Tuple a){
    return Tuple(-a.x, -a.y, -a.z, -a.point);
}

// Multiplies a tuple by a factor of scale
Tuple multiplyTuple(Tuple a, float scale){
    return Tuple(a.x*scale, a.y*scale, a.z*scale, a.point*scale);
}

// Divides a tuple by scale
Tuple divideTuple(Tuple a, float scale){
    return multiplyTuple(a, 1/scale);
}

// Point constructor
Point::Point(float x, float y, float z): Tuple(x, y, z, 1.0) {}

// Vector constructor
Vector::Vector(float x, float y, float z): Tuple(x, y, z, 0.0) {}

// NOTE!! Point is included in these vector operations to hopefully help if there
// are any bugs later as point should always be 0 so it will have no effect on these

// Calculates magnitude of vector
float Vector::magnitude(){
    return sqrt(x*x + y*y + z*z + point*point);
}

// Normalizes vector so it's magnitude = 1
void Vector::normalize(){
    float mag = magnitude();

    x = x/mag;
    y = y/mag;
    z = z/mag;
    point = point/mag;
}

// Calculates dot product of vectors a and b
float dotProduct(Vector a, Vector b){
    return a.x*b.x + a.y*b.y + a.z*b.z + a.point*b.point;
}

// Calculates cross product of vectors a and b
Vector crossProduct(Vector a, Vector b){
    return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}