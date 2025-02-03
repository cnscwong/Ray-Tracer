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
    return std::vector<Intersection>{};
}

// Computes the normal vector of a point on the surface of the shape
// findIntersections does some preprocessing that would be done for any shape
Vector Shape::computeNormal(Point p){
    // Converts point p to a point relative to the shape(what p would be if shape origin was at 0)
    Point shape_point((transform.inverse()*p));
    // Computes a normal relative to the sphere
    Vector shape_normal = childNormal(shape_point);
    // Converts the computed normal back to points relative to the world
    Vector world_normal = (transform.inverse().transpose()*shape_normal);
    world_normal.point = 0;

    return world_normal.normalize();
}

// childIntersections executes custom code depending on what child class is being executed
Vector Shape::childNormal(Point p){
    return Vector();
}

// Shape equality function
bool Shape::isEqual(Shape* s){
    return transform.isEqual(s->getTransform()) && material.isEqual(s->getMaterial());
}

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
        return origin.isEqual(s1->getOrigin()) && floatIsEqual(radius, s1->getRadius()) && Shape::isEqual(s);
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
    return sphere_normal;
}

// Generates a sphere with a glass material
Sphere* glassSphere(){
    Material m;
    m.transparency = 1;
    m.refractiveIndex = 1.5;
    Sphere* s = new Sphere;
    s->setMaterial(m);

    return s;
}

// Computes the point of intersection of a ray on the plane 
std::vector<Intersection> Plane::childIntersections(Ray r){
    // Since the default plane is an xz plane before transformation, any vector with a y value of ~0(floating-point error) will be parallel to the plane
    // A coplanar ray is a ray that is parallel to the plane and originates on the plane, this ray intersects the plane at every single point
    // This will return zero intersections because if this is the input ray, the camera is viewing the plane edge-on. Since the plane is infinitely thin
    // nothing should be rendered so no need to return any intersections
    if(std::abs(r.getDirection().y) < EPSILON){
        return std::vector<Intersection>{};
    }

    // computes the time the ray takes to travel -y units in the y direction(time = distance/speed) so that the ray is on the plane(y value is 0)
    float t = -r.getOrigin().y/r.getDirection().y;
    return std::vector<Intersection>{Intersection(t, this)};
}

// The default plane is an xz plane, so the normal vector will be Vector(0, 1, 0)
Vector Plane::childNormal(Point p){
    return Vector(0, 1, 0);
}

// Computes all times a ray hits the cube
std::vector<Intersection> Cube::childIntersections(Ray r){
    // Computes the times when the ray intersected with the corresponding plane of each face of the cube
    std::vector<float> xt_minmax = check_axis(r.getOrigin().x, r.getDirection().x);
    std::vector<float> yt_minmax = check_axis(r.getOrigin().y, r.getDirection().y);
    std::vector<float> zt_minmax = check_axis(r.getOrigin().z, r.getDirection().z);

    // The largest min time and smallest max time will always be the times the ray intersects with the cube
    float tmin = std::max({xt_minmax.at(0), yt_minmax.at(0), zt_minmax.at(0)});
    float tmax = std::min({xt_minmax.at(1), yt_minmax.at(1), zt_minmax.at(1)});

    // Ray does not intersect with cube
    if(tmin > tmax){
        return std::vector<Intersection>();
    }

    return std::vector<Intersection>({Intersection(tmin, this), Intersection(tmax, this)});
}

// Computes the normal vector of a point on the cube. For a cube at the origin with a side length of 2,
// it's normal vector will correspond to the max absolute value of all components on the point.
// eg. Point(1, 0.5, -0.8) will be on the +x side of the cube and will have a normal of (1, 0, 0)
Vector Cube::childNormal(Point p){
    // maxComponent cannot be set to 1.0 just in case there is floating point error
    float maxComponent = std::max({std::abs(p.x), std::abs(p.y), std::abs(p.z)});

    if(maxComponent == std::abs(p.x)){
        return Vector(p.x, 0, 0);
    }else if(maxComponent == std::abs(p.y)){
        return Vector(0, p.y, 0);
    }

    return Vector(0, 0, p.z);
}

// Computes the time that the ray hits the plane corresponding to a negative and positive face of a cube using time = distance/speed 
// where speed is the direction parameter passed in and distance will be calculated using the origin parameter
// eg. Calculates when a ray hits a plane at x=-1 and x=1 to determine if the intersection was on the cube's surface
std::vector<float> check_axis(float origin, float direction){
    // Distance from origin to the plane x = -1 or x = 1 if origin corresponds to the cube's origin.x
    float tmin_numerator = -1 - origin;
    float tmax_numerator = 1 - origin;

    float tmin, tmax;

    if(std::abs(direction) >= EPSILON){
        tmin = tmin_numerator/direction;
        tmax = tmax_numerator/direction;
    }else{ // if direction is near 0, handles division by 0
        tmin = tmin_numerator*INFINITY;
        tmax = tmax_numerator*INFINITY;
    }

    if(tmin > tmax){
        std::swap(tmin, tmax);
    }

    return std::vector<float>({tmin, tmax});
}