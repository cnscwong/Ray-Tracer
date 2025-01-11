#include "Ray.h"

// Ray constructors
Ray::Ray(){
    origin = Point();
    direction = Vector();
}

Ray::Ray(Point o, Vector d){
    origin = o;
    direction = d;
}

// Returns private variables origin and direction
Point Ray::getOrigin(){
    return origin;
}

Vector Ray::getDirection(){
    return direction;
}

// Computes the position of the ray at time t
Tuple Ray::computePosition(float t){
    return (origin + (direction*t));
}

// Returns a vector of intersections where the ray intersects the surface of the sphere s
// Eg. A ray that originates from (-3, 0, 0) and travels at speed (1, 0, 0) will
// intersect a sphere with radius 1 and originates from (0, 0, 0) at {2, 4}
// where time = 2 is when the ray first hits the sphere at (-1, 0 , 0) and
// exits the sphere at time = 4 at point (1, 0, 0)
// Search about "Line-sphere intersection" for more info on how the math works
std::vector<Intersection> Ray::RaySphereIntersection(Sphere s){
    // Any transform that we want to apply to the sphere has to be applied inversely to the ray
    // if we want the same result as transforming the sphere
    Ray ray2 = this->transform(s.getTransform().inverse());

    // Vector from spheres center to the ray origin
    Vector sphere_to_ray = Vector(ray2.getOrigin() - s.getOrigin());
    float a = dotProduct(ray2.getDirection(), ray2.getDirection());
    float b = 2*dotProduct(ray2.getDirection(), sphere_to_ray);
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

    return std::vector<Intersection>{Intersection(t1, s), Intersection(t2, s)};
}

// Transforms the ray by the matrix m
Ray Ray::transform(Matrix m){
    return Ray(Point(m*origin), Vector(m*direction));
}