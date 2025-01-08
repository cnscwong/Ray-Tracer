#include "Sphere.h"

int Sphere::ID = 0;

// Sphere constructors
Sphere::Sphere(){
    radius = 1;
    origin = Point();
    id = ++ID;
}

// Getters for sphere variables
float Sphere::getRadius(){
    return radius;
}

Point Sphere::getOrigin(){
    return origin;
}

Matrix Sphere::getTransform(){
    return transform;
}

Material Sphere::getMaterial(){
    return material;
}

// Setter for sphere transform variable
void Sphere::setTransform(Matrix m){
    transform = m;
}

void Sphere::setMaterial(Material m){
    material = m;
}

// Checks equality of spheres
bool Sphere::isEqual(Sphere s){
    return id == s.id && origin.isEqual(s.getOrigin()) && floatIsEqual(radius, s.getRadius()) && transform.isEqual(s.getTransform());
}

// Computes the normal vector at the point p on the surface of the sphere
// The normal vector is the vector that is perpendicular to the surface of the sphere
// and has a magnitude equal to 1(normalized). Assume point p is always on surface of sphere
Vector Sphere::computeNormal(Point p){
    // Converts point p to a point relative to the sphere(what p would be if sphere origin was at 0)
    Point sphere_point((transform.inverse()*p));
    // Computes the normal vector relative to the sphere
    Vector sphere_normal((sphere_point - Point(0, 0, 0)));
    // Computes sphere normal vector so that it is relative to the world(3d coord system)
    Vector world_normal((transform.inverse().transpose()*sphere_normal));
    return world_normal.normalize();
}