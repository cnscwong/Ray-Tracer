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
Colour World::shadeHit(LightData data, int remaining){
    bool shadowed = data.object->getMaterial().castsShadow && hasShadow(data.overPoint);
    Colour surfaceCol = computeLighting(data.object->getMaterial(), data.object, light, data.overPoint, data.camera, data.normal, shadowed);
    Colour reflectedCol = reflectedColour(data, remaining);
    Colour refractedCol = refractedColour(data, remaining);

    Material m = data.object->getMaterial();
    if(m.reflective > 0 && m.transparency > 0){
        float reflectance = schlickApproximation(data);
        return surfaceCol + reflectedCol*reflectance + refractedCol*(1 - reflectance);
    }

    return surfaceCol + reflectedCol + refractedCol;
}

// Computes the colour at the first point hit by the ray r
Colour World::colourAtHit(Ray r, int remaining){
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
    LightData data = prepareLightData(intersects.at(ind), r, intersects);
    return this->shadeHit(data, remaining);
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

// Computes colour of a reflective surface in the world when it is hit by a ray
Colour World::reflectedColour(LightData data, int remaining){
    if(data.object->getMaterial().reflective == 0 || remaining <= 0){
        return BLACK;
    }

    Ray reflectRay(data.overPoint, data.reflect);
    Colour c = colourAtHit(reflectRay, remaining - 1);

    return c*data.object->getMaterial().reflective;
}

// Computes colour of a surface when hit by a ray based on the material's transparency and refractive properties
Colour World::refractedColour(LightData data, int remaining){
    if(data.object->getMaterial().transparency == 0 || remaining == 0){
        return BLACK;
    }

    // Check for total internal reflection using Snell's law. When a refracted ray reflects back into the material 
    // it was previously in due to the refractive indices of the materials and the angle the ray hits the other material
    // Ratio of n1 and n2
    double n_ratio = data.n1/data.n2;
    // Finds cos(theta_i) using the dot product of the two vectors
    double cos_i = dotProduct(data.camera, data.normal);
    // Finds sin(theta_t)^2 using trig identity
    double sin2_t = pow(n_ratio, 2)*(1.0-pow(cos_i, 2));
    // total internal reflection occurs
    if(sin2_t > 1){
        return BLACK;
    }

    // Compute refracted colour if no edge cases
    // Finds cos(theta_t) using trig identity
    double cos_t = sqrt(1.0 - sin2_t);
    // Compute direction of refracted ray
    Vector direction = data.normal*(n_ratio*cos_i - cos_t) - data.camera*n_ratio;
    Ray refractedRay(data.underPoint, direction);
    // Finds colour of refracted ray
    Colour c = colourAtHit(refractedRay, remaining - 1)*data.object->getMaterial().transparency;

    // Multiplies by transparency value to account for any opacity
    return c;
}

// Creates a default world with a light source and two spheres
World defaultWorld(){
    World w;

    Shape* s1 = new Sphere;
    Material m;
    m.colour = Colour(0.8, 1.0, 0.6);
    m.diffuse = 0.7;
    m.specular = 0.2;
    s1->setMaterial(m);

    Shape* s2 = new Sphere;
    s2->setTransform(scalingMatrix(0.5, 0.5, 0.5));

    LightSource light(Point(-10, 10, -10), Colour(1, 1, 1));

    w.appendObject(s1);
    w.appendObject(s2);
    w.setLight(light);

    return w;
}