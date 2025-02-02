#pragma once
#include "Shape.h"
#include "Intersection.h"
#include "Ray.h"
#include "Tuple.h"
#include "LightAndShading.h"
#include "Colour.h"
#include <vector>

// Class to store computeLighting parameter data
class LightData{
public:
    // Object being hit
    Shape* object;
    // Time at which object is hit
    float time;
    // The point where the ray hits the object
    Point point;
    // Overpoint is close to point and is used for shadows
    // Overpoint is offset a fraction above the surface
    // This is used so the renderer doesn't think a point casts a point on itself due to bit error
    Point overPoint;
    // Underpoint is offset a fraction below the surface
    Point underPoint;
    // Vector pointing from the point to the camera
    Vector camera;
    // Normal vector at the point
    Vector normal;
    // Reflection vector of the ray on the point
    Vector reflect;
    // Boolean for whether the ray is inside the object before it hit the object
    bool insideObject;
    // Floats corresponding to the refractive index of the material being exited(n1) and entered(n2)
    float n1, n2;
    
    LightData();
};

// Takes an intersection and ray and prepares them for computeLighting function
// The rayIntersects vector stores all the intersections of the ray passed in to prepare refraction data
LightData prepareLightData(Intersection i, Ray r, std::vector<Intersection> rayIntersects = std::vector<Intersection>());
void findRefractiveIndices(LightData &data, Intersection i, std::vector<Intersection> rayIntersects = std::vector<Intersection>());
// Approximating Fresnel Effect using Schlick's approximation to find the reflectance which represents the fraction of light 
// that is reflected, used
float schlickApproximation(LightData data);