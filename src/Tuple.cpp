#include "Tuple.h"

// Tuple constructors
Tuple::Tuple(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->point = 1;
}

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
Tuple Tuple::operator+(Tuple b){
    return Tuple(x + b.x, y + b.y, z + b.z, point + b.point);
};

// Performs a - b. Intuitively, subtracting a point from a point generates a vector from p2 to p1. Subtracting a point
// from a vector moves the point back the vector's distance and direction. Subtracting two vectors represents the change
// in direction between the two.
Tuple Tuple::operator-(Tuple b){
    return Tuple(x - b.x, y - b.y, z - b.z, point - b.point);
}

// Multiplies a tuple by a factor of scale
Tuple Tuple::operator*(float scale){
    return Tuple(x*scale, y*scale, z*scale, point*scale);
}

// Divides a tuple by scale
Tuple Tuple::operator/(float scale){
    return Tuple(x/scale, y/scale, z/scale, point/scale);
}

// Negates a tuple. Flipping direction of a vector
Tuple Tuple::negateTuple(){
    return Tuple(-x, -y, -z, -point);
}

// Point constructors
Point::Point(): Tuple(0, 0, 0, 1.0) {}

Point::Point(float x, float y, float z): Tuple(x, y, z, 1.0) {}

Point::Point(Tuple t): Tuple(t.x, t.y, t.z, 1.0) {}

// Vector constructors
Vector::Vector(): Tuple(1, 1, 1, 0.0) {}

Vector::Vector(float x, float y, float z): Tuple(x, y, z, 0.0) {}

Vector::Vector(Tuple t): Tuple(t.x, t.y, t.z, 0.0) {}

// NOTE!! Point is included in these vector operations to hopefully help if there
// are any bugs later as point should always be 0 so it will have no effect on these

// Calculates magnitude of vector
float Vector::magnitude(){
    return sqrt(x*x + y*y + z*z + point*point);
}

// Normalizes vector so it's magnitude = 1
Vector Vector::normalize(){
    float mag = magnitude();

    return Vector(x/mag, y/mag, z/mag);
}

// Calculates dot product of vectors a and b
float dotProduct(Vector a, Vector b){
    return a.x*b.x + a.y*b.y + a.z*b.z + a.point*b.point;
}

// Calculates cross product of vectors a and b
Vector crossProduct(Vector a, Vector b){
    return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}