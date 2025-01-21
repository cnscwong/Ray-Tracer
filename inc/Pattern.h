#pragma once
#include "Colour.h"
#include "Tuple.h"
#include "common.h"
#include "Matrix.h"
#include <vector>
class Shape; // forward declaration of shape

// Parent class for patterns. Children will be custom patterns that can be applied to objects
// The transform is used to manipulate the pattern on objects(eg. make it larger, rotate it)
class Pattern{
public:
    Matrix transform = Matrix(4);

    Matrix getTransform();
    void setTransform(Matrix m);

    Colour applyPattern(Shape* s, Point p);
    virtual Colour ChildApplyPattern(Point p);
};

// Will apply stripes to the object
class Stripes : public Pattern{
public:
    std::vector<Colour> stripes;

    Stripes();
    Stripes(std::initializer_list<Colour> colours);

    // Pattern override
    Colour ChildApplyPattern(Point p);
};