
#pragma once
#include "Sphere.h"
#include "Intersection.h"
#include "Ray.h"
#include "Tuple.h"
#include "LightAndShading.h"
#include "Colour.h"

// Class to store computeLighting parameter data
class LightData{
private:
public:
    Sphere object;
    float time;
    Point point;
    // Overpoint is close to point and is used for shadows
    // Overpoint is a little bit farther away from the shape 
    // than point. This is so that the hasShadow function does
    // not accidentally find itself when searching for intersections
    Point overPoint;
    Vector camera;
    Vector normal;
    bool insideObject;
    
    LightData();
    LightData(Sphere o, float t, Point p, Vector e, Vector n);
};

// Takes an intersection and ray and prepares them for computeLighting function
LightData prepareLightData(Intersection i, Ray r);