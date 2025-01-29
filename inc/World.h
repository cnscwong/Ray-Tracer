#pragma once
#include "LightAndShading.h"
#include "Matrix.h"
#include "Colour.h"
#include "Tuple.h"
#include "Intersection.h"
#include <vector>
#include "Ray.h"
#include "LightData.h"
#include "Config.h"
#include "Shape.h"

// Class to store all objects in the environment
class World{
private:
    // Stores all objects in the world and the light source
    std::vector<Shape*> objects;
    LightSource light;
public:
    // World constructor
    World();

    // Getters and setters for variables
    std::vector<Shape*> getObjects();
    LightSource getLight();

    void appendObject(Shape* s);
    void setLight(LightSource l);
    void setObjects(std::vector<Shape*> obj);

    // Returns a vector of intersection objects where the ray r intersects the surface of an object in the world
    std::vector<Intersection> RayIntersection(Ray r);
    // Returns the computed colour of a hit using the world light source and the LightData data structure
    Colour shadeHit(LightData data, int remaining = RECURSIVE_REFLECT_LIMIT);
    // Computes the colour at the first point hit by the ray r
    Colour colourAtHit(Ray r, int remaining = RECURSIVE_REFLECT_LIMIT);
    // Checks if a point p in the world is covered by a shadow(object between point and light source)
    bool hasShadow(Point p);
    // Computes the reflected colour using LightData and the material's reflective attribute
    Colour reflectedColour(LightData data, int remaining = RECURSIVE_REFLECT_LIMIT);
};

// Returns a default world with a light source and two spheres
World defaultWorld();
