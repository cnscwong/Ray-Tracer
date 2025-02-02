#pragma once
#include <vector>
class Shape; // forward declarations

// Class that stores the time and sphere that the intersection occurred at
class Intersection{
    private:
        // Stores time and sphere that a ray intersected
        float time;
        Shape* s;
    public:
        // Intersection constructor
        Intersection(float t, Shape* s);

        // Getters for Intersection variables
        float getTime();
        Shape* getShape();

        // Equality check
        bool isEqual(Intersection i);
};

// Function to pack a list of intersections into a vector
std::vector<Intersection> intersections(std::initializer_list<Intersection> l);

// Takes a list of intersections and identifies the index of the first object that
// the ray hits in the list of objects. The object that is hit is the object with 
// the lowest non-negative time
int hit(std::vector<Intersection> vec);

// Intersection comparison function
bool compareIntersections(Intersection a, Intersection b);
