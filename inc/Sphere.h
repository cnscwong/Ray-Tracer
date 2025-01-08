#pragma once
#include "Tuple.h"
#include "Matrix.h"
#include "LightAndShading.h"

// Class to represent spheres in the canvas, stores origin and radius, origin is center of sphere
class Sphere{
    protected:
        // Variable to keep track of allocated ids
        static int ID;
    private:
        Point origin;
        float radius;
        Matrix transform = Matrix(4);
        Material material;
    public:
        // Stores sphere ids
        int id;
        // Sphere constructors
        Sphere();

        // Getters for sphere variables
        float getRadius();
        Point getOrigin();
        Matrix getTransform();
        Material getMaterial();

        // Setter for transform variable
        void setTransform(Matrix m);
        void setMaterial(Material m);

        // Checks if sphere is equal to s
        bool isEqual(Sphere s);

        // Computes the normal vector of a point on the surface of the sphere
        Vector computeNormal(Point p);
};
