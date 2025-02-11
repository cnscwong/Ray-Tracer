#pragma once
#include "Shape.h"
#include "Intersection.h"
#include "Tuple.h"
#include <vector>

// Class storing a group of shapes, useful for designing objects at the origin and than transforming them after
class Group : public Shape{
private:
    // Stores all shapes contained in the group
    std::vector<Shape*> shapes;
public:
    std::vector<Shape*> getShapes();
    void appendShape(Shape* s);

    // Shape override function
    std::vector<Intersection> childIntersections(Ray r);
};