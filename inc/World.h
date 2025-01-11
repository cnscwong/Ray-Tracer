#pragma once
#include "Sphere.h"
#include "LightAndShading.h"
#include "Matrix.h"
#include "Colour.h"
#include "Tuple.h"
#include "Intersection.h"
#include <vector>

// Class to store all objects in the environment
class World{
private:
    // Stores all objects in the world and the light source
    std::vector<Sphere> objects;
    LightSource light;
public:
    // World constructors
    World();

    // Getters and setters for variables
    std::vector<Sphere> getObjects();
    LightSource getLight();

    void appendObject(Sphere s);
    void setLight(LightSource l);
};

// Returns a default world with a light source and two spheres
World defaultWorld();
