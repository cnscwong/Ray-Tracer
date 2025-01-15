#pragma once
#include "Tuple.h"
#include "Matrix.h"
#include <vector>
#include <algorithm>

// Class for rays and ray operations
class Ray{
    private:
        // Stores the coordinates the ray originates from
        Point origin;
        // Stores the direction and speed the ray travels in one time unit
        Vector direction;
    public:
        // Ray constructors
        Ray();
        Ray(Point o, Vector d);

        // Getters
        Point getOrigin();
        Vector getDirection();

        // Computes the position of the ray at time t
        Tuple computePosition(float t);
        
        // Returns a ray that is transformed by the matrix m
        Ray transform(Matrix m);
};
