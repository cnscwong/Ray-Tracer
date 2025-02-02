#include "LightAndShading.h"

// Computes reflection vector of an input vector bouncing
// off a point on  surface where the parameter normal is
// the normal vector of the surface at that point
Vector reflectVector(Vector input, Vector normal){
    return Vector(input - normal*2*dotProduct(input, normal));
}

// Light source constructors
LightSource::LightSource(){
    position = Point(0, 0, 0);
    intensity = Colour(0, 0, 0);
}

LightSource::LightSource(Point p, Colour i){
    position = p;
    intensity = i;
}

// Light source getters
Point LightSource::getPosition(){
    return position;
}

Colour LightSource::getIntensity(){
    return intensity;
}

// Equality function
bool LightSource::isEqual(LightSource l){
    return position.isEqual(l.getPosition()) && intensity.isEqual(l.getIntensity());
}

// Material constructors
Material::Material(){
    colour = Colour(1, 1, 1);
    pattern = nullptr;
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200;
    reflective = 0;
    transparency = 0;
    refractiveIndex = 1;
    castsShadow = true;
}

// Material equality checker
bool Material::isEqual(Material m){
    if(!colour.isEqual(m.colour)){
        return false;
    }
    if(!floatIsEqual(ambient, m.ambient)){
        return false;
    }
    if(!floatIsEqual(diffuse, m.diffuse)){
        return false;
    }
    if(!floatIsEqual(specular, m.specular)){
        return false;
    }
    if(!floatIsEqual(shininess, m.shininess)){
        return false;
    }
    if(!floatIsEqual(reflective, m.reflective)){
        return false;
    }
    if(!floatIsEqual(transparency, m.transparency)){
        return false;
    }
    if(!floatIsEqual(refractiveIndex, m.refractiveIndex)){
        return false;
    }
    if(castsShadow != m.castsShadow){
        return false;
    }

    return true;
}

// Calculates the updated colour value of a point based on the ray, light, and object/material attributes
Colour computeLighting(Material m, Shape* object, LightSource l, Point p, Vector camera, Vector normal, bool inShadow){
    Colour colour;
    if(m.pattern == nullptr){
        colour = m.colour;
    }else{
        colour = m.pattern->applyPattern(object, p);
    }

    // Combines the material colour and light colour together
    Colour combinedColour = colour*l.getIntensity();

    // Computes ambient contribution
    Colour ambient = combinedColour*m.ambient;

    if(inShadow){
        return ambient;
    }

    // Vector from the point to the light source
    Vector lightVector = Vector((l.getPosition() - p)).normalize();

    // Dot product of light and normal vector, also represents cosine of angle between light and normal
    // vectors. Negative dot product means the light is on the other side of the surface relative to the 
    // perspective(camera position)
    float LNdot = dotProduct(lightVector, normal);
    Colour diffuse, specular;

    // If light is on other side of surface, diffuse and specular set to black
    if(LNdot < 0){
        diffuse = Colour(0, 0, 0);
        specular = Colour(0, 0, 0);
    }else{
        // Computes diffuse contribution
        diffuse = combinedColour*m.diffuse*LNdot;

        // Reflected vector of light that bounces on p
        Vector reflection = reflectVector(Vector(lightVector.negateTuple()), normal);
        // Dot product of light and camera vector. Cosine of angle between reflection vector and camera vector
        // Negative value means that the reflection points away from the camera
        float REdot = dotProduct(reflection, camera);

        // If light reflection points away from camera, specular set to black
        if(REdot <= 0){
            specular = Colour(0, 0, 0);
        }else{
            // Computes specular contribution
            float sFactor = pow(REdot, m.shininess);
            specular = l.getIntensity()*m.specular*sFactor;
        }
    }

    return ambient + diffuse + specular;
}