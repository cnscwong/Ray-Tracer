#pragma once
#include "Sphere.h"
#include <vector>

// Class that stores the time and sphere that the intersection occurred at
class Intersection{
    private:
        // Stores time and sphere that a ray intersected
        float time;
        Sphere s;
    public:
        // Intersection constructor
        Intersection(float t, Sphere s);

        // Getters for Intersection variables
        float getTime();
        Sphere getSphere();
};

// Function to pack a list of intersections into a vector
std::vector<Intersection> intersections(std::initializer_list<Intersection> l);

// Takes a list of intersections and identifies the index of the first object that
// the ray hits in the list of objects. The object that is hit is the object with 
// the lowest non-negative time
int hit(std::vector<Intersection> vec);
