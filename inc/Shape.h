#pragma once
#include "Matrix.h"
#include "LightAndShading.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Ray.h"

// Parent class for all objects that can be rendered
class Shape{
protected:
    // Stores material of shape and the matrix transformation that is applied to the shape
    Matrix transform = Matrix(4);
    Material material;
public:
    // Getter and setter for transform and material
    Matrix getTransform();
    void setTransform(Matrix m);
    Material getMaterial();
    void setMaterial(Material m);

    // Returns a vector of intersection objects where the ray r intersects the surface of the shape
    // findIntersections does some preprocessing that would be done for any shape
    std::vector<Intersection> findIntersections(Ray r);
    // childIntersections executes custom code depending on what child class is being executed
    virtual std::vector<Intersection> childIntersections(Ray r);

    // Computes the normal vector of a point on the surface of the shape
    // findIntersections does some preprocessing that would be done for any shape
    Vector computeNormal(Point p);
    // childIntersections executes custom code depending on what child class is being executed
    virtual Vector childNormal(Point p);

    virtual bool isEqual(Shape* s);
};

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

Sphere* glassSphere();

// class to store the plane shape, a flat surface that extends infinitely in two directions and has no thickness
class Plane : public Shape{
public:
    // Shape class override functions
    // Computes the point of intersection of a ray on the plane 
    std::vector<Intersection> childIntersections(Ray r);
    // The normal vector at any point on the plane is the same
    // The default plane is an xz plane, so the normal vector will be Vector(0, 1, 0)
    Vector childNormal(Point p);
};

// class to represent cubes
class Cube : public Shape{
public:
    // Shape class override functions
    std::vector<Intersection> childIntersections(Ray r);
    Vector childNormal(Point p);
};

// Cube helper function for computing intersections
std::vector<float> check_axis(float origin, float direction);