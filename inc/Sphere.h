#pragma once
#include "Tuple.h"
#include "Matrix.h"
#include "LightAndShading.h"
#include "Shape.h"
#include "Ray.h"
#include "Intersection.h"
#include <vector>

// Class to represent spheres in the canvas, stores origin and radius, origin is center of sphere
class Sphere: public Shape{
    private:
        Point origin;
        float radius;
    public:
        // Stores sphere ids
        int id;
        // Sphere constructors
        Sphere();

        // Getters for sphere variables
        float getRadius();
        Point getOrigin();

        // Checks if sphere is equal to s
        bool isEqual(Shape* s);

        // Shape class override functions
        // Computes all intersections of the ray r with the sphere
        std::vector<Intersection> childIntersections(Ray r);
        // Computes normal vector at point p on the sphere
        Vector childNormal(Point p);
};
