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
    c = Colour(1, 1, 1);
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200;
}

// Material getters
Colour Material::getColour(){
    return c;
}

float Material::getAmbient(){
    return ambient;
}

float Material::getDiffuse(){
    return diffuse;
}

float Material::getSpecular(){
    return specular;
}

float Material::getShininess(){
    return shininess;
}

// Material setters
void Material::setColour(Colour col){
    this->c = col;
}

void Material::setAmbient(float a){
    if(a < 0){
        throw std::invalid_argument("Material variable should be nonnegative");
    }

    ambient = a;
}

void Material::setDiffuse(float d){
    if(d < 0){
        throw std::invalid_argument("Material variable should be nonnegative");
    }

    diffuse = d;
}

void Material::setSpecular(float s){
    if(s < 0){
        throw std::invalid_argument("Material variable should be nonnegative");
    }

    specular = s;
}

void Material::setShininess(float s){
    if(s < 0){
        throw std::invalid_argument("Material variable should be nonnegative");
    }

    shininess = s;
}

// Material equality checker
bool Material::isEqual(Material m){
    if(!c.isEqual(m.getColour())){
        return false;
    }

    if(!floatIsEqual(ambient, m.getAmbient())){
        return false;
    }

    if(!floatIsEqual(diffuse, m.getDiffuse())){
        return false;
    }

    if(!floatIsEqual(specular, m.getSpecular())){
        return false;
    }

    if(!floatIsEqual(shininess, m.getShininess())){
        return false;
    }

    return true;
}

// Calculates the updated colour value of a point based on the ray, light, and object/material attributes
Colour computeLighting(Material m, Point p, LightSource l, Vector eye, Vector normal){
    // Combines the material colour and light colour together
    Colour combinedColour = m.getColour()*l.getIntensity();

    // Vector from the point to the light source
    Vector lightVector = Vector((l.getPosition() - p)).normalize();

    // Computes ambient contribution
    Colour ambient = combinedColour*m.getAmbient();

    // Dot product of light and normal vector, also represents cosine of angle between light and normal
    // vectors. Negative dot product means the light is on the other side of the surface relative to the 
    // perspective(eye position)
    float LNdot = dotProduct(lightVector, normal);
    Colour diffuse, specular;

    // If light is on other side of surface, diffuse and specular set to black
    if(LNdot < 0){
        diffuse = Colour(0, 0, 0);
        specular = Colour(0, 0, 0);
    }else{
        // Computes diffuse contribution
        diffuse = combinedColour*m.getDiffuse()*LNdot;

        // Reflected vector of light that bounces on p
        Vector reflection = reflectVector(Vector(lightVector.negateTuple()), normal);
        // Dot product of light and eye vector. Cosine of angle between reflection vector and eye vector
        // Negative value means that the reflection points away from the eye
        float REdot = dotProduct(reflection, eye);

        // If light reflection points away from eye, specular set to black
        if(REdot <= 0){
            specular = Colour(0, 0, 0);
        }else{
            // Computes specular contribution
            float sFactor = pow(REdot, m.getShininess());
            specular = l.getIntensity()*m.getSpecular()*sFactor;
        }
    }

    return ambient + diffuse + specular;
}