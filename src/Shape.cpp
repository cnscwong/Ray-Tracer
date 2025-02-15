#include "Shape.h"
#include "Group.h"

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

Group* Shape::getParent(){
    return parent;
}

void Shape::setParent(Group* p){
    parent = p;
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
    Point objectPoint = worldToObject(p);
    Vector objectNormal = childNormal(objectPoint);
    return normalToWorld(objectNormal);
}

// childIntersections executes custom code depending on what child class is being executed
Vector Shape::childNormal(Point p){
    return Vector();
}

// Shape equality function
bool Shape::isEqual(Shape* s){
    return transform.isEqual(s->getTransform()) && material.isEqual(s->getMaterial());
}

// Converts a point in the world to a point relative to the shape
// eg. Converts the point to where it would be if the shape was at the origin
Point Shape::worldToObject(Point p){
    if(parent != nullptr){
        p = parent->worldToObject(p);
    }

    return transform.inverse()*p;
}

Vector Shape::normalToWorld(Vector normal){
    normal = Vector(transform.inverse().transpose()*normal);
    normal = normal.normalize();

    if(parent != nullptr){
        normal = parent->normalToWorld(normal);
    }
    return normal;
}

// Checks equality of spheres
bool Sphere::isEqual(Shape* s){
    Sphere* s1 = dynamic_cast<Sphere*>(s);
    if(s1 == nullptr){
        return false;
    }else{
        return Shape::isEqual(s);
    }
}

// Eg. A ray that originates from (-3, 0, 0) and travels at speed (1, 0, 0) will
// intersect a sphere with radius 1 and originates from (0, 0, 0) at {2, 4}
// where time = 2 is when the ray first hits the sphere at (-1, 0 , 0) and
// exits the sphere at time = 4 at point (1, 0, 0)
// Search about "Line-sphere intersection" for more info on how the math works
std::vector<Intersection> Sphere::childIntersections(Ray r){
    // Vector from spheres center to the ray origin
    Vector sphere_to_ray = Vector(r.getOrigin() - Point());
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

// Computes all intersections of a ray and the cube
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

// Cylinder constructor
Cylinder::Cylinder(){
    maxH = INFINITY;
    minH = -INFINITY;
    closed = false;
}

// Getters and setters
float Cylinder::getMaxH(){
    return maxH;
}

float Cylinder::getMinH(){
    return minH;
}

bool Cylinder::getClosed(){
    return closed;
}

void Cylinder::setMaxH(float h){
    if(minH > h){
        throw std::invalid_argument("Cylinder:setMaxH - Invalid input: " + std::to_string(h));
    }else{
        maxH = h;
    }
}

void Cylinder::setMinH(float h){
    if(maxH < h){
        throw std::invalid_argument("Cylinder:setMinH - Invalid input: " + std::to_string(h));
    }else{
        minH = h;
    }
}

void Cylinder::setClosed(bool c){
    closed = c;
}

// Computes all intersections of a ray and the cylinder
std::vector<Intersection> Cylinder::childIntersections(Ray r){
    float a = pow(r.getDirection().x, 2) + pow(r.getDirection().z, 2);

    std::vector<Intersection> intersects;
    // If a is approximately 0, ray does not intersect with cylinder walls
    if(std::abs(a) < EPSILON){
        intersectCaps(r, intersects);
        return intersects;
    }

    float b = 2*r.getOrigin().x*r.getDirection().x + 2*r.getOrigin().z*r.getDirection().z;
    float c = pow(r.getOrigin().x, 2) + pow(r.getOrigin().z, 2) - 1;

    float t0;
    float t1;
    float discriminant = pow(b, 2) - 4*a*c;
    if(floatIsEqual(discriminant, 0)){
        t0 = -b/(2*a);
        t1 = -b/(2*a);
    }else if(discriminant < 0){
        // Ray does not intersect if discriminant is negative
        return std::vector<Intersection>();
    }else{
        t0 = (-b - sqrt(discriminant))/(2*a);
        t1 = (-b + sqrt(discriminant))/(2*a);
    }

    if(t0 > t1){
        std::swap(t0, t1);
    }

    // Computes y values of intersections and checks if they are within cylinder top and bottom bounds
    float y0 = r.getOrigin().y + t0*r.getDirection().y;
    if(minH < y0 && y0 < maxH){
        intersects.push_back(Intersection(t0, this));
    }
    float y1 = r.getOrigin().y + t1*r.getDirection().y;
    if(minH < y1 && y1 < maxH){
        intersects.push_back(Intersection(t1, this));
    }

    // Add intersections with cylinder caps
    intersectCaps(r, intersects);

    return intersects;
}

// Returns normal vector of a point on the cylinder walls or caps(if closed cylinder)
Vector Cylinder::childNormal(Point p){
    // Calculates the square of the distance of the point from the y axis, if distance = 1 point is on wall of cylinder
    float distance = pow(p.x, 2) + pow(p.z, 2);

    // If point is on top cap(first if case), if point is on bottom cap(second if case)
    // Utilizes EPSILON to avoid any floating point error of the components of point p
    if(distance < 1 && p.y >= maxH - EPSILON){
        return Vector(0, 1, 0);
    }else if(distance < 1 && p.y <= minH + EPSILON){
        return Vector(0, -1, 0);
    }

    return Vector(p.x, 0, p.z);
}

// Checks if ray r at time t is inside the radius of the cylinder
bool Cylinder::insideCapRadius(Ray r, float t){
    float x = r.getOrigin().x + t*r.getDirection().x;
    float z = r.getOrigin().z + t*r.getDirection().z;

    return (pow(x, 2) + pow(z, 2)) <= 1;
}

// Computes ray intersection with cylinder caps
void Cylinder::intersectCaps(Ray r, std::vector<Intersection> &intersects){
    // If cylinder is not closed or ray is travelling parallel to y, intersection never happens
    // Ignores case when ray is on cylinder cap as there will be infinite intersections
    if(!closed || floatIsEqual(r.getDirection().y, 0)){
        return;
    }

    // Calculates time when ray is level with the bottom cap of the cylinder
    float t = (minH - r.getOrigin().y)/r.getDirection().y;
    if(insideCapRadius(r, t)){
        intersects.push_back(Intersection(t, this));
    }

    // Calculates time when ray is level with the top cap of the cylinder
    t = (maxH - r.getOrigin().y)/r.getDirection().y;
    if(insideCapRadius(r, t)){
        intersects.push_back(Intersection(t, this));
    }
}

// Cone constructor
Cone::Cone(){
    maxH = INFINITY;
    minH = -INFINITY;
    closed = false;
}

// Getters and setters
float Cone::getMaxH(){
    return maxH;
}

float Cone::getMinH(){
    return minH;
}

bool Cone::getClosed(){
    return closed;
}

void Cone::setMaxH(float h){
    if(minH > h){
        throw std::invalid_argument("Cone:setMaxH - Invalid input: " + std::to_string(h));
    }else{
        maxH = h;
    }
}

void Cone::setMinH(float h){
    if(maxH < h){
        throw std::invalid_argument("Cone:setMinH - Invalid input: " + std::to_string(h));
    }else{
        minH = h;
    }
}

void Cone::setClosed(bool c){
    closed = c;
}

// Computes all intersections of a ray and the cone
std::vector<Intersection> Cone::childIntersections(Ray r){
    float a = pow(r.getDirection().x, 2) - pow(r.getDirection().y, 2) + pow(r.getDirection().z, 2);
    float b = 2*r.getOrigin().x*r.getDirection().x - 2*r.getOrigin().y*r.getDirection().y + 2*r.getOrigin().z*r.getDirection().z;
    float c = pow(r.getOrigin().x, 2) - pow(r.getOrigin().y, 2) + pow(r.getOrigin().z, 2);

    std::vector<Intersection> intersects;
    // If a is approximately 0, ray does not intersect with cone walls
    if(std::abs(a) < EPSILON){
        intersectCaps(r, intersects);
        if(std::abs(b) < EPSILON){
            return intersects;
        }
        intersects.push_back(Intersection(-c/(2*b), this));
        return intersects;
    }

    float t0;
    float t1;
    float discriminant = pow(b, 2) - 4*a*c;
    if(floatIsEqual(discriminant, 0)){
        t0 = -b/(2*a);
        t1 = -b/(2*a);
    }else if(discriminant < 0){
        // Ray does not intersect if discriminant is negative
        return std::vector<Intersection>();
    }else{
        t0 = (-b - sqrt(discriminant))/(2*a);
        t1 = (-b + sqrt(discriminant))/(2*a);
    }

    if(t0 > t1){
        std::swap(t0, t1);
    }

    // Computes y values of intersections and checks if they are within cylinder top and bottom bounds
    float y0 = r.getOrigin().y + t0*r.getDirection().y;
    if(minH < y0 && y0 < maxH){
        intersects.push_back(Intersection(t0, this));
    }
    float y1 = r.getOrigin().y + t1*r.getDirection().y;
    if(minH < y1 && y1 < maxH){
        intersects.push_back(Intersection(t1, this));
    }

    // Add intersections with cylinder caps
    intersectCaps(r, intersects);

    return intersects;
}

// Returns normal vector of a point on the cone walls or caps(if closed cone)
Vector Cone::childNormal(Point p){
    // Calculates the square of the distance of the point from the y axis, if distance = 1 point is on wall of cone
    float distance = pow(p.x, 2) + pow(p.z, 2);

    // If point is on top cap(first if case), if point is on bottom cap(second if case)
    // Utilizes EPSILON to avoid any floating point error of the components of point p
    if(distance < pow(maxH, 2) && p.y >= maxH - EPSILON){
        return Vector(0, 1, 0);
    }else if(distance < pow(minH, 2) && p.y <= minH + EPSILON){
        return Vector(0, -1, 0);
    }

    float y = sqrt(pow(p.x, 2) + pow(p.z, 2));
    if(p.y > 0){
        return Vector(p.x, -y, p.z);
    }
    return Vector(p.x, y, p.z);
}

// Checks if ray r at time t is inside the radius of the cone
bool Cone::insideCapRadius(Ray r, float t, float radius){
    float x = r.getOrigin().x + t*r.getDirection().x;
    float z = r.getOrigin().z + t*r.getDirection().z;

    return (pow(x, 2) + pow(z, 2)) <= pow(radius, 2);
}

// Computes ray intersection with cone caps
void Cone::intersectCaps(Ray r, std::vector<Intersection> &intersects){
    // If cone is not closed or ray is travelling parallel to y, intersection never happens
    // Ignores case when ray is on cone cap as there will be infinite intersections
    if(!closed || floatIsEqual(r.getDirection().y, 0)){
        return;
    }

    // Calculates time when ray is level with the bottom cap of the cone
    float t = (minH - r.getOrigin().y)/r.getDirection().y;
    if(insideCapRadius(r, t, minH)){
        intersects.push_back(Intersection(t, this));
    }

    // Calculates time when ray is level with the top cap of the cone
    t = (maxH - r.getOrigin().y)/r.getDirection().y;
    if(insideCapRadius(r, t, maxH)){
        intersects.push_back(Intersection(t, this));
    }
}

// Triangle constructor
Triangle::Triangle(Point p1, Point p2, Point p3){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    e1 = p2 - p1;
    e2 = p3 - p1;
    normal = crossProduct(e2, e1).normalize(); 
}

// Triangle getters
Point Triangle::getP1(){
    return p1;
}

Point Triangle::getP2(){
    return p2;
}

Point Triangle::getP3(){
    return p3;
}

Vector Triangle::getE1(){
    return e1;
}

Vector Triangle::getE2(){
    return e2;
}

Vector Triangle::getNormal(){
    return normal;
}

// Moller-Trumbore ray-triangle intersection algorithm
std::vector<Intersection> Triangle::childIntersections(Ray r){
    Vector dir_cross_e2 = crossProduct(r.getDirection(), e2);
    float det = dotProduct(e1, dir_cross_e2);

    // Ray is parallel to triangle
    if(std::abs(det) < EPSILON){
        return std::vector<Intersection>();
    }

    // Ray misses p1-p3 edge
    float f = 1.0/det;
    Vector p1_to_origin = r.getOrigin() - p1;
    float u = f*dotProduct(p1_to_origin, dir_cross_e2);
    if(u < 0 || u > 1){
        return std::vector<Intersection>();
    }

    // Ray misses p1-p2 edge and ray misses the p2-p3 edge
    Vector origin_cross_e1 = crossProduct(p1_to_origin, e1);
    float v = f*dotProduct(r.getDirection(), origin_cross_e1);
    if(v < 0 || (u + v) > 1){
        return std::vector<Intersection>();
    }

    // Ray hits the triangle
    float t = f*dotProduct(e2, origin_cross_e1);
    return std::vector<Intersection>({Intersection(t, this)});
}

// The normal on any point of the triangle is the precomputed normal
Vector Triangle::childNormal(Point p){
    return normal;
}