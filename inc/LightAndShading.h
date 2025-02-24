#pragma once
#include "Tuple.h"
#include "Colour.h"
#include <stdexcept>
#include "common.h"
#include "Pattern.h"

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
    // Lightsource constructors
    LightSource();
    LightSource(Point p, Colour i);

    // Variable getters
    Point getPosition();
    Colour getIntensity();

    // Equality function
    bool isEqual(LightSource l);
};

// Class representing a material and storing attributes for the Phong
// reflection model. The material has a colour c. Ambient refers to
// light reflected from other objects in the environment. Diffuse refers
// to light reflected from a matte surface, dependent on the light source
// and the surface normal. Specular refers the reflection of the light source
// itself, depends on the reflection vector and the camera vector(Vector pointing 
// from a p to the origin of the ray) point that the rendered image is captured 
// from. Shininess controls the brightness of the specular reflection
class Material{
private:
public:
    Colour colour;
    Pattern* pattern;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    float reflective;
    float transparency;
    float refractiveIndex;
    bool castsShadow;

    // Material constructor
    Material();

    // Equality function
    bool isEqual(Material m);
};

// Performs lighting computations. Takes the material, the point that is being lit,
// the light source, camera vector, and normal vector as input parameters.
// Also, considers if the point has a shadow casted on it by another object
Colour computeLighting(Material m, Shape* object, LightSource l, Point p, Vector camera, Vector normal, bool inShadow);