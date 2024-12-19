#include "Tuple.h"

Tuple::Tuple(float x, float y, float z, float point){
    this->x = x;
    this->y = y;
    this->z = z;
    this->point = point;
}

float Tuple::isPoint(){
    return this->point;
}

Point::Point(float x, float y, float z): Tuple(x, y, z, 1.0f) {}

Vector::Vector(float x, float y, float z): Tuple(x, y, z, 0.0f) {}