#include "Sphere.h"

// Sphere constructors
Sphere::Sphere(){
    radius = 1;
    origin = Point();
}

// Getters for sphere variables
float Sphere::getRadius(){
    return radius;
}

Point Sphere::getOrigin(){
    return origin;
}

// Checks equality of spheres
bool Sphere::isEqual(Shape* s){
    Sphere* s1 = dynamic_cast<Sphere*>(s);
    if(s1 == nullptr){
        return false;
    }else{
        return origin.isEqual(s1->getOrigin()) && floatIsEqual(radius, s1->getRadius()) && transform.isEqual(s1->getTransform()) && material.isEqual(s1->getMaterial());
    }
}

// Eg. A ray that originates from (-3, 0, 0) and travels at speed (1, 0, 0) will
// intersect a sphere with radius 1 and originates from (0, 0, 0) at {2, 4}
// where time = 2 is when the ray first hits the sphere at (-1, 0 , 0) and
// exits the sphere at time = 4 at point (1, 0, 0)
// Search about "Line-sphere intersection" for more info on how the math works
std::vector<Intersection> Sphere::childIntersections(Ray r){
    // Vector from spheres center to the ray origin
    Vector sphere_to_ray = Vector(r.getOrigin() - origin);
    float a = dotProduct(r.getDirection(), r.getDirection());
    float b = 2*dotProduct(r.getDirection(), sphere_to_ray);
    float c = dotProduct(sphere_to_ray, sphere_to_ray) - 1;
    // Discriminant of quadratic formula using values computed above
    float discriminant = b*b - 4*a*c;

    // If discriminant negative, no intersection
    if(discriminant < 0){
        return std::vector<Intersection>{};
    }

    // Otherwise, the result is the two results of the quadratic formula
    // If the ray is tangent to the spheres surface and only intersects the
    // sphere at one point, t1 will be equal to t2
    float t1 = (-b - sqrt(discriminant))/(2*a);
    float t2 = (-b + sqrt(discriminant))/(2*a);

    return std::vector<Intersection>{Intersection(t1, this), Intersection(t2, this)};
}

// Computes the normal vector at the point p on the surface of the sphere
// The normal vector is the vector that is perpendicular to the surface of the sphere
// and has a magnitude equal to 1(normalized). Assume point p is always on surface of sphere
Vector Sphere::childNormal(Point p){
    // Computes the normal vector relative to the sphere
    Vector sphere_normal((p - Point(0, 0, 0)));
    // Computes sphere normal vector so that it is relative to the world(3d coord system)
    Vector world_normal((transform.inverse().transpose()*sphere_normal));
    return world_normal.normalize();
}