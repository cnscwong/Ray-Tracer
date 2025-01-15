#include "Shape.h"

// Getter and setter for transform and material
Matrix Shape::getTransform(){
    return transform;
}

void Shape::setTransform(Matrix m){
    transform = m;
}

Material Shape::getMaterial(){
    return material;
}

void Shape::setMaterial(Material m){
    material = m;
}

// Returns a vector of intersections where the ray intersects the surface of the shape
// findIntersections does some preprocessing that would be done for any shape
std::vector<Intersection> Shape::findIntersections(Ray r){
    // Any transform that we want to apply to the shape has to be applied inversely to the ray
    // if we want the same result as transforming the shape
    Ray ray2 = r.transform(transform.inverse());

    return childIntersections(ray2);
}

// childIntersections executes custom code depending on what child class is being executed
std::vector<Intersection> Shape::childIntersections(Ray r){
    return std::vector<Intersection>();
}

// Computes the normal vector of a point on the surface of the shape
// findIntersections does some preprocessing that would be done for any shape
Vector Shape::computeNormal(Point p){
    // Converts point p to a point relative to the shape(what p would be if shape origin was at 0)
    Point shape_point((transform.inverse()*p));

    return childNormal(shape_point);
}

// childIntersections executes custom code depending on what child class is being executed
Vector Shape::childNormal(Point p){
    return Vector();
}

// Shape equality function
bool Shape::isEqual(Shape* s){
    return false;
}