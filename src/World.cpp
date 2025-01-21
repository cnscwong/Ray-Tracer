#include "World.h"

// World constructor
World::World(){
    light = LightSource();
}

// Gets the list of objects in the world
std::vector<Shape*> World::getObjects(){
    return objects;
}

// Gets the light source in the world
LightSource World::getLight(){
    return light;
}

// Adds an object to the world
void World::appendObject(Shape* s){
    objects.push_back(s);
}

// Sets the light source
void World::setLight(LightSource l){
    light = l;
}

// Sets the objects in the world
void World::setObjects(std::vector<Shape*> obj){
    objects = obj;
}

// Returns a vector of intersections where the ray intersects the surface of the objects in the world
std::vector<Intersection> World::RayIntersection(Ray r){
    // Initializes the intersection vectors needed to compute the intersections
    std::vector<Intersection> intersects;
    std::vector<Intersection> temp;

    // Adds the intersections of all the objects with the ray into
    // the intersects vector
    for(int i = 0; i < objects.size(); i++){
        temp = objects.at(i)->findIntersections(r);
        for(int j = 0; j < temp.size(); j++){
            intersects.push_back(temp.at(j));
        }
    }

    std::sort(intersects.begin(), intersects.end(), compareIntersections);

    return intersects;
}

// Returns the computed colour of a hit using the world light source and the LightData data structure
Colour World::shadeHit(LightData data){
    return computeLighting(data.object->getMaterial(), data.object, light, data.overPoint, data.camera, data.normal, hasShadow(data.overPoint));
}

// Computes the colour at the first point hit by the ray r
Colour World::colourAtHit(Ray r){
    // Find intersections of ray at hit
    std::vector<Intersection> intersects = this->RayIntersection(r);

    if(intersects.size() == 0){
        return Colour();
    }

    int ind = -1;
    for(int i = 0; i < intersects.size(); i++){
        if(intersects.at(i).getTime() >= 0){
            ind = i;
            break;
        }
    }

    if(ind == -1){
        return Colour();
    }

    // Uses object that is hit first
    LightData data = prepareLightData(intersects.at(ind), r);
    return this->shadeHit(data);
}

// Checks if a point has an object covering the light source
bool World::hasShadow(Point p){
    if(!RENDER_SHADOWS){
        return false;
    }

    Vector v = Vector((light.getPosition() - p));
    float distance = v.magnitude();
    Vector direction = v.normalize();

    Ray r(p, direction);
    std::vector<Intersection> intersects = this->RayIntersection(r);

    int ind = hit(intersects);
    if(ind != -1 && intersects.at(ind).getTime() < distance){
        return true;
    }else{
        return false;
    }
}

// Creates a default world with a light source and two spheres
World defaultWorld(){
    World w;

    Shape* s1 = new Sphere;
    Material m;
    m.setColour(Colour(0.8, 1.0, 0.6));
    m.setDiffuse(0.7);
    m.setSpecular(0.2);
    s1->setMaterial(m);

    Shape* s2 = new Sphere;
    s2->setTransform(scalingMatrix(0.5, 0.5, 0.5));

    LightSource light(Point(-10, 10, -10), Colour(1, 1, 1));

    w.appendObject(s1);
    w.appendObject(s2);
    w.setLight(light);

    return w;
}