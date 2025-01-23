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
    std::vector<Colour> colours = std::vector<Colour>({WHITE, BLACK});
    Matrix transform = Matrix(4);

    Matrix getTransform();
    void setTransform(Matrix m);

    Colour applyPattern(Shape* s, Point p);
    virtual Colour ChildApplyPattern(Point p);
};

class Stripes : public Pattern{
public:
    Stripes();
    Stripes(std::initializer_list<Colour> colours);

    // Pattern override
    Colour ChildApplyPattern(Point p);
};

class LinearGradient : public Pattern{
public:
    LinearGradient();
    LinearGradient(std::initializer_list<Colour> colours);

    // Pattern override
    Colour ChildApplyPattern(Point p);
};

// Depends on x and z value, think of a archery target
class RingPattern : public Pattern{
public:
    RingPattern();
    RingPattern(std::initializer_list<Colour> colours);

    // Pattern override
    Colour ChildApplyPattern(Point p);
};

// Depends on x, y, and z value
class CheckerPattern : public Pattern{
public:
    CheckerPattern();
    CheckerPattern(std::initializer_list<Colour> colours);

    // Pattern override
    Colour ChildApplyPattern(Point p);
};