#pragma once
#include "common.h"
#include <cmath>

// Parent class for points and vectors
class Tuple{
    public:
        float x, y, z;
        // A variable to store the state of the tuple(1.0 for point, 0.0 for vector)
        // This is a float because some operations involving a tuple and other vectors, points, and matrices
        // mathematically recompute to be 1.0 or 0.0 which is consistent to what the final state of the
        // tuple should be(point or vector). Just think of this as a 4d tuple (x, y, z, point)
        float point;

        Tuple(float x, float y, float z, float point);
        bool isEqual(Tuple a);
};

// Tuple Operations
Tuple addTuples(Tuple a, Tuple b);
Tuple subtractTuples(Tuple a, Tuple b);
Tuple negateTuple(Tuple a);
Tuple multiplyTuple(Tuple a, float scale);
Tuple divideTuple(Tuple a, float scale);

// Class for a point, inherits from Tuple
class Point: public Tuple{
    public:
        Point(float x, float y, float z);
};

// Class for a vector, inherits from Tuple
class Vector: public Tuple{
    public:
        Vector(float x, float y, float z);

        // Vector Operations
        float magnitude();
        void normalize();
};

// More Vector Operations
float dotProduct(Vector a, Vector b);
Vector crossProduct(Vector a, Vector b);
