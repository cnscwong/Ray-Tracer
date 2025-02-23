#pragma once
#include "Shape.h"
#include <stdexcept>
#include <vector>

// The set operation options for a CSG, using enum class for type safety
enum class SetOperation {
    UNION,
    INTERSECTION,
    DIFFERENCE
};

#define UNION SetOperation::UNION
#define INTERSECTION SetOperation::INTERSECTION
#define DIFFERENCE SetOperation::DIFFERENCE

// Class representing Constructive Solid Geometry(CSG). CSG shapes are similar to groups where they are considered parents of their children shapes
// CSG combines shapes with the set operations: union, intersection, and difference
// CSG operations are binary(consist of 2 shapes) and will be implemented using a binary tree to allow for nested set operations
class CSG : public Shape{
private:
    Shape* left;
    Shape* right;
    SetOperation op;
public:
    // CSG constructor
    CSG(SetOperation op, Shape* l, Shape* r);

    // Getters
    Shape* getLeft();
    Shape* getRight();
    SetOperation getOp();

    // Determines if the intersection being evaluated is valid(visible to the camera, eg. not inside one of the shapes)
    bool validIntersection(bool hitLeft, bool insideLeft, bool insideRight);
    // Filters out all invalid intersections from a list of intersections that
    std::vector<Intersection> filterIntersections(std::vector<Intersection> intersects);

    // Shape override functions
    bool includes(Shape* s);
    std::vector<Intersection> childIntersections(Ray r);
};