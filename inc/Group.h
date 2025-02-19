#pragma once
#include "Shape.h"
#include "Intersection.h"
#include "Tuple.h"
#include <vector>
#include <string>

// Class storing a group of shapes, useful for designing objects at the origin and than transforming them after
class Group : public Shape{
private:
    // Stores all shapes contained in the group
    std::vector<Shape*> shapes;
public:
    // Group name, used for obj parser
    std::string name = "";

    Group();
    Group(std::string n);

    // getters and setters
    std::vector<Shape*> getShapes();
    void appendShape(Shape* s);

    // Shape override function
    std::vector<Intersection> childIntersections(Ray r);
};