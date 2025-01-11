#pragma once
#include "Sphere.h"
#include "LightAndShading.h"
#include "Matrix.h"
#include "Colour.h"
#include "Tuple.h"
#include "Intersection.h"
#include <vector>
#include "Ray.h"
#include "LightData.h"

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
    void setObjects(std::vector<Sphere> obj);

    // Returns a vector of intersection objects where the ray r intersects the surface of an object in the world
    std::vector<Intersection> RayIntersection(Ray r);
    // Returns the computed colour of a hit using the world light source and the LightData data structure
    Colour shadeHit(LightData data);
    // Computes the colour at the first point hit by the ray r
    Colour colourAtHit(Ray r);
};

// Returns a default world with a light source and two spheres
World defaultWorld();
