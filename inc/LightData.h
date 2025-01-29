#pragma once
#include "Shape.h"
#include "Intersection.h"
#include "Ray.h"
#include "Tuple.h"
#include "LightAndShading.h"
#include "Colour.h"

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
    // Overpoint is a little bit farther away from the shape 
    // than point. This is so that the hasShadow function does
    // not accidentally find itself when searching for intersections
    Point overPoint;
    // Vector pointing from the point to the camera
    Vector camera;
    // Normal vector at the point
    Vector normal;
    // Reflection vector of the ray on the point
    Vector reflect;
    // Boolean for whether the ray is inside the object before it hit the object
    bool insideObject;
    
    LightData();
    LightData(Shape* o, float t, Point p, Vector e, Vector n);
};

// Takes an intersection and ray and prepares them for computeLighting function
LightData prepareLightData(Intersection i, Ray r);