#pragma once
#include "Tuple.h"
#include "Colour.h"
#include <stdexcept>
#include "common.h"

// File to store functions to simulate light reflection and shading

// Takes a vector input that is hitting a surface at a point and 
// the normal vector at that point. Computes the resulting vector 
// that would be produced when the input vector reflects or bounces
// off of the surface
Vector reflectVector(Vector input, Vector normal);

// Class representing a light source originating from a single point
class LightSource{
private:
    // Position that the light source is located
    Point position;
    // Colour and intensity of the light
    Colour intensity;
public:
    // Lightsource constructor
    LightSource(Point p, Colour i);

    // Variable getters
    Point getPosition();
    Colour getIntensity();
};

// Class representing a material and storing attributes for the Phong
// reflection model. The material has a colour c. Ambient refers to
// light reflected from other objects in the environment. Diffuse refers
// to light reflected from a matte surface, dependent on the light source
// and the surface normal. Specular refers the reflection of the light source
// itself, depends on the reflection vector and the eye vector(Vector pointing 
// from a p to the origin of the ray) point that the rendered image is captured 
// from. Shininess controls the brightness of the specular reflection
class Material{
private:
    Colour c;
    float ambient, diffuse, specular, shininess;
public:
    // Material constructor
    Material();

    // Variable getters and setters
    Colour getColour();
    float getAmbient();
    float getDiffuse();
    float getSpecular();
    float getShininess();

    void setColour(Colour col);
    void setAmbient(float a);
    void setDiffuse(float d);
    void setSpecular(float s);
    void setShininess(float s);

    // Equality function
    bool isEqual(Material m);
};

// Performs lighting computations. Takes the material, the point that is being lit,
// the light source, eye vector, and normal vector as input parameters
Colour computeLighting(Material m, Point p, LightSource l, Vector eye, Vector normal);