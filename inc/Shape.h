#pragma once
#include "Matrix.h"
#include "LightAndShading.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Ray.h"
#include <stdexcept>
class Group;
// Forward declaration of group because group is a child of shape and contains shapes
// A shape can have a group it belongs to

// Parent class for all objects that can be rendered
class Shape{
protected:
    // Stores material of shape and the matrix transformation that is applied to the shape
    Matrix transform = Matrix(4);
    Material material = Material();
    Group* parent = nullptr;
public:
    // Getter and setter for transform and material
    Matrix getTransform();
    void setTransform(Matrix m);
    Material getMaterial();
    void setMaterial(Material m);
    Group* getParent();
    void setParent(Group* p);

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

    // Equality check function
    virtual bool isEqual(Shape* s);
    
    // Recursive functions for groups
    // Converts a point in the world to a point relative to the shape
    // Utilizes the shape's transform as well as any parent group transforms
    Point worldToObject(Point p);
    // Converts a normal vector relative to the shape to a vector in the world coordinates
    Vector normalToWorld(Vector normal);
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

// Class to represent cubes, default cube has a side length of 2 and origin at Point(0, 0, 0)
class Cube : public Shape{
public:
    // Shape class override functions
    std::vector<Intersection> childIntersections(Ray r);
    Vector childNormal(Point p);
};

// Cube helper function for computing intersections
std::vector<float> check_axis(float origin, float direction);

// Class to represent cylinders, the default cylinder extends infinitely in the +y and -y direction on the y axis
class Cylinder : public Shape{
private:
    // y values of top and bottom of default cylinder(these may not correspond for a transformed cylinder)
    float maxH;
    float minH;
    // boolean determining whether the cylinder has top and bottom caps or is hollow
    bool closed;
public:
    // Constructor
    Cylinder();

    // Getters and setters
    float getMaxH();
    float getMinH();
    bool getClosed();
    void setMaxH(float h);
    void setMinH(float h);
    void setClosed(bool c);

    // Shape class override functions
    std::vector<Intersection> childIntersections(Ray r);
    Vector childNormal(Point p);

    // Intersection helper functions for the top and bottom caps
    static bool insideCapRadius(Ray r, float t);
    void intersectCaps(Ray r, std::vector<Intersection> &intersects);
};

// Class to represent cones, the default cone extends infinitely in the +y and -y direction on the y axis
class Cone : public Shape{
private:
    // y values of top and bottom of default cone(these may not correspond for a transformed cone)
    float maxH;
    float minH;
    // boolean determining whether the cone has top and bottom caps or is hollow
    bool closed;
public:
    // Constructor
    Cone();

    // Getters and setters
    float getMaxH();
    float getMinH();
    bool getClosed();
    void setMaxH(float h);
    void setMinH(float h);
    void setClosed(bool c);

    // Shape class override functions
    std::vector<Intersection> childIntersections(Ray r);
    Vector childNormal(Point p);

    // Intersection helper functions for the top and bottom caps
    static bool insideCapRadius(Ray r, float t, float radius);
    void intersectCaps(Ray r, std::vector<Intersection> &intersects);
};