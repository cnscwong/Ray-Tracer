#include "World.h"

// World constructor
World::World(){
    light = LightSource();
}

// Gets the list of objects in the world
std::vector<Sphere> World::getObjects(){
    return objects;
}

// Gets the light source in the world
LightSource World::getLight(){
    return light;
}

// Adds an object to the world
void World::appendObject(Sphere s){
    objects.push_back(s);
}

// Sets the light source
void World::setLight(LightSource l){
    light = l;
}

// Creates a default world with a light source and two spheres
World defaultWorld(){
    World w;

    Sphere s1;
    Material m;
    m.setColour(Colour(0.8, 1.0, 0.6));
    m.setDiffuse(0.7);
    m.setSpecular(0.2);
    s1.setMaterial(m);

    Sphere s2;
    s2.setTransform(scalingMatrix(0.5, 0.5, 0.5));

    LightSource light(Point(-10, 10, -10), Colour(1, 1, 1));

    w.appendObject(s1);
    w.appendObject(s2);
    w.setLight(light);

    return w;
}