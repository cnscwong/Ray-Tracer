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