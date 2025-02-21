#include <gtest/gtest.h>
#include "LightAndShading.h"
#include "LightData.h"
#include "Shape.h"

TEST(SphereTest, NormalVectorTest){
    Shape* s = new Sphere;
    EXPECT_TRUE(s->computeNormal(Point(1, 0, 0)).isEqual(Vector(1, 0, 0)));
    EXPECT_TRUE(s->computeNormal(Point(0, 1, 0)).isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(s->computeNormal(Point(0, 0, 1)).isEqual(Vector(0, 0, 1)));
    EXPECT_TRUE(s->computeNormal(Point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)).isEqual(Vector(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)));
    Vector temp = s->computeNormal(Point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3));
    EXPECT_TRUE(temp.isEqual(temp.normalize()));
    s->setTransform(translationMatrix(0, 1, 0));
    EXPECT_TRUE(s->computeNormal(Point(0, 1.70711, -0.70711)).isEqual(Vector(0, 0.70711, -0.70711)));
    s->setTransform((scalingMatrix(1, 0.5, 1)*zRotationMatrix(PI/5)));
    EXPECT_TRUE(s->computeNormal(Point(0, sqrt(2)/2, -sqrt(2)/2)).isEqual(Vector(0, 0.97014, -0.24254)));
    delete s;
}

TEST(LightShadingTest, ReflectionTest){
    Vector v(1, -1, 0);
    Vector n(0, 1, 0);
    Vector r = reflectVector(v, n);
    EXPECT_TRUE(r.isEqual(Vector(1, 1, 0)));

    v = Vector(0, -1, 0);
    n = Vector(sqrt(2)/2, sqrt(2)/2, 0);
    r = reflectVector(v, n);
    EXPECT_TRUE(r.isEqual(Vector(1, 0, 0)));
}

TEST(LightSourceTest, BasicTest){
    LightSource light(Point(0, 0, 0), Colour(1, 1, 1));
    EXPECT_TRUE(light.getPosition().isEqual(Point(0, 0, 0)));
    EXPECT_TRUE(light.getIntensity().isEqual(Colour(1, 1, 1)));
}

TEST(MaterialTest, BasicTest){
    Material m;
    EXPECT_TRUE(m.colour.isEqual(Colour(1, 1, 1)));
    EXPECT_TRUE(floatIsEqual(m.ambient, 0.1));
    EXPECT_TRUE(floatIsEqual(m.diffuse, 0.9));
    EXPECT_TRUE(floatIsEqual(m.specular, 0.9));
    EXPECT_TRUE(floatIsEqual(m.shininess, 200));
    EXPECT_TRUE(floatIsEqual(m.reflective, 0));
    EXPECT_TRUE(floatIsEqual(m.transparency, 0));
    EXPECT_TRUE(floatIsEqual(m.refractiveIndex, 1));

    Sphere s;
    m = s.getMaterial();
    EXPECT_TRUE(m.isEqual(Material()));

    m.ambient = 1;
    EXPECT_TRUE(floatIsEqual(m.ambient, 1));
    s.setMaterial(m);
    EXPECT_TRUE(s.getMaterial().isEqual(m));
}

// Light       Camera         ||
TEST(LightingTest, CameraBetweenSurfaceAndLight){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Camera and normal vectors
    Vector camera(0, 0, -1);
    Vector normal(0, 0, -1);
    // Light behind the camera
    LightSource light(Point(0, 0, -10), Colour(1, 1, 1));

    Shape* s = new Shape;

    Colour result = computeLighting(m, s, light, position, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(1.9, 1.9, 1.9)));
    delete s;
    s = nullptr;
}

//           Camera
// 
// 
// 
// Light                ||
// Camera is 45 degrees off of the normal vector
TEST(LightingTest, CameraAboveSurfaceAndLight){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Camera and normal vectors
    Vector camera(0, sqrt(2)/2, -sqrt(2)/2);
    Vector normal(0, 0, -1);
    // Light behind the camera
    LightSource light(Point(0, 0, -10), Colour(1, 1, 1));

    Shape* s = new Shape;

    Colour result = computeLighting(m, s, light, position, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(1, 1, 1)));
    delete s;
    s = nullptr;
}

//           Light
// 
// 
// 
//           Camera         ||
// Camera is opposite the surface, light is 45 degrees off the normal vector
TEST(LightingTest, LightAboveSurfaceAndCamera){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Camera and normal vectors
    Vector camera(0, 0, -1);
    Vector normal(0, 0, -1);
    // Light behind the camera
    LightSource light(Point(0, 10, -10), Colour(1, 1, 1));

    Shape* s = new Shape;

    Colour result = computeLighting(m, s, light, position, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(0.7364, 0.7364, 0.7364)));
    delete s;
    s = nullptr;
}

//           Light
// 
// 
// 
//                      ||
// 
// 
// 
// 
//             Camera
// light is 45 degrees off the normal vector, camera is in position
// where light is reflected into it(Also 45 degrees off normal vector)
TEST(LightingTest, LightAboveSurfaceCameraBelowSurface){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Camera and normal vectors
    Vector camera(0, -sqrt(2)/2, -sqrt(2)/2);
    Vector normal(0, 0, -1);
    // Light behind the camera
    LightSource light(Point(0, 10, -10), Colour(1, 1, 1));

    Shape* s = new Shape;

    Colour result = computeLighting(m, s, light, position, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(1.6364, 1.6364, 1.6364)));
    delete s;
    s = nullptr;
}

// Camera               ||       Light
// Light is behind surface, camera is opposite surface
TEST(LightingTest, LightBehindSurface){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Camera and normal vectors
    Vector camera(0, 0, -1);
    Vector normal(0, 0, -1);
    // Light behind the camera
    LightSource light(Point(0, 0, 10), Colour(1, 1, 1));

    Shape* s = new Shape;

    Colour result = computeLighting(m, s, light, position, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(0.1, 0.1, 0.1)));
    delete s;
    s = nullptr;
}

TEST(LightDataTest, PrepareLightDataTest){
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere* s = new Sphere;
    Intersection i(4, s);
    LightData data = prepareLightData(i, r);
    EXPECT_EQ(data.time, 4);
    EXPECT_TRUE(data.object->isEqual(s));
    EXPECT_TRUE(data.point.isEqual(Point(0, 0, -1)));
    EXPECT_TRUE(data.camera.isEqual(Vector(0, 0, -1)));
    EXPECT_TRUE(data.normal.isEqual(Vector(0, 0, -1)));
    EXPECT_EQ(data.insideObject, false);

    r = Ray(Point(), Vector(0, 0, 1));
    i = Intersection(1, s);
    data = prepareLightData(i, r);
    EXPECT_TRUE(data.point.isEqual(Point(0, 0, 1)));
    EXPECT_TRUE(data.camera.isEqual(Vector(0, 0, -1)));
    EXPECT_TRUE(data.normal.isEqual(Vector(0, 0, -1)));
    EXPECT_EQ(data.insideObject, true);
    delete s;
}

// Test for the overPoint variable in the LightData 
// Checks that it is a little bit above the object surface
TEST(LightDataTest, OverpointTest){
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(0, 0, 1));
    Intersection i(5, s);
    LightData data = prepareLightData(i, r);
    EXPECT_TRUE(data.overPoint.z < -EPSILON/2);
    EXPECT_TRUE(data.point.z > data.overPoint.z);
    delete s;
}

TEST(LightDataTest, ReflectVectorTest){
    Plane* p = new Plane;
    Ray r(Point(0, 1, -1), Vector(0, -sqrt(2)/2, sqrt(2)/2));
    Intersection i(sqrt(2), p);
    LightData data = prepareLightData(i, r);
    EXPECT_TRUE(data.reflect.isEqual(Vector(0, sqrt(2)/2, sqrt(2)/2)));
}

TEST(FindRefractiveIndicesTest, RayPassesFromNothingToObject){
    Material m;
    m.refractiveIndex = 1.5;
    Sphere* A = glassSphere();
    A->setMaterial(m);
    m.refractiveIndex = 2;
    Sphere* B = glassSphere();
    B->setMaterial(m);
    m.refractiveIndex = 2.5;
    Sphere* C = glassSphere();
    C->setMaterial(m);
    std::vector<Intersection> RayIntersects({Intersection(2, A), Intersection(2.75, B), Intersection(3.25, C), Intersection(4.75, B), Intersection(5.25, C), Intersection(6, A)});
    LightData data;

    findRefractiveIndices(data, RayIntersects.at(0), RayIntersects);

    EXPECT_EQ(data.n1, 1.0);
    EXPECT_EQ(data.n2, 1.5);
}

TEST(FindRefractiveIndicesTest, RayPassesFromObjectToObject){
    Material m;
    m.refractiveIndex = 1.5;
    Sphere* A = glassSphere();
    A->setMaterial(m);
    m.refractiveIndex = 2;
    Sphere* B = glassSphere();
    B->setMaterial(m);
    m.refractiveIndex = 2.5;
    Sphere* C = glassSphere();
    C->setMaterial(m);
    std::vector<Intersection> RayIntersects({Intersection(2, A), Intersection(2.75, B), Intersection(3.25, C), Intersection(4.75, B), Intersection(5.25, C), Intersection(6, A)});
    LightData data1;
    LightData data2;
    LightData data3;
    LightData data4;

    findRefractiveIndices(data1, RayIntersects.at(1), RayIntersects);
    findRefractiveIndices(data2, RayIntersects.at(2), RayIntersects);
    findRefractiveIndices(data3, RayIntersects.at(3), RayIntersects);
    findRefractiveIndices(data4, RayIntersects.at(4), RayIntersects);

    EXPECT_EQ(data1.n1, 1.5);
    EXPECT_EQ(data1.n2, 2.0);
    EXPECT_EQ(data2.n1, 2.0);
    EXPECT_EQ(data2.n2, 2.5);
    EXPECT_EQ(data3.n1, 2.5);
    EXPECT_EQ(data3.n2, 2.5);
    EXPECT_EQ(data4.n1, 2.5);
    EXPECT_EQ(data4.n2, 1.5);
}

TEST(FindRefractiveIndicesTest, RayPassesFromObjectToNothing){
    Material m;
    m.refractiveIndex = 1.5;
    Sphere* A = glassSphere();
    A->setMaterial(m);
    m.refractiveIndex = 2;
    Sphere* B = glassSphere();
    B->setMaterial(m);
    m.refractiveIndex = 2.5;
    Sphere* C = glassSphere();
    C->setMaterial(m);
    std::vector<Intersection> RayIntersects({Intersection(2, A), Intersection(2.75, B), Intersection(3.25, C), Intersection(4.75, B), Intersection(5.25, C), Intersection(6, A)});
    LightData data;

    findRefractiveIndices(data, RayIntersects.at(5), RayIntersects);

    EXPECT_EQ(data.n1, 1.5);
    EXPECT_EQ(data.n2, 1.0);
}

// Test for the underPoint variable in the LightData 
// Checks that it is a little bit below the object surface
TEST(LightDataTest, UnderpointTest){
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere* s = glassSphere();
    s->setTransform(translationMatrix(0, 0, 1));
    Intersection i(5, s);
    LightData data = prepareLightData(i, r);
    EXPECT_TRUE(data.underPoint.z > -EPSILON/2);
    EXPECT_TRUE(data.point.z < data.underPoint.z);
    delete s;
}

TEST(LightDataTest, SchlickApproximationReturnsOneWhenTotalInternalReflectionOccurs){
    Sphere* s = glassSphere();
    Ray r(Point(0, 0, sqrt(2)/2), Vector(0, 1, 0));
    std::vector<Intersection> intersects({Intersection(-sqrt(2)/2, s), Intersection(sqrt(2)/2, s)});
    LightData data = prepareLightData(intersects.at(1), r, intersects);
    float reflectance = schlickApproximation(data);

    EXPECT_EQ(reflectance, 1.0);
}

TEST(LightDataTest, SchlickApproximationIsSmallWhenRayPerpendicularToSurface){
    Sphere* s = glassSphere();
    Ray r(Point(0, 0, 0), Vector(0, 1, 0));
    std::vector<Intersection> intersects({Intersection(-1, s), Intersection(1, s)});
    LightData data = prepareLightData(intersects.at(1), r, intersects);
    float reflectance = schlickApproximation(data);

    EXPECT_TRUE(floatIsEqual(reflectance, 0.04));
}

TEST(LightDataTest, SchlickApproximationWhenRayHitsSurfaceAtASmallAngle){
    Sphere* s = glassSphere();
    Ray r(Point(0, 0.99, -2), Vector(0, 0, 1));
    std::vector<Intersection> intersects({Intersection(1.8589, s)});
    LightData data = prepareLightData(intersects.at(0), r, intersects);
    float reflectance = schlickApproximation(data);

    EXPECT_TRUE(floatIsEqual(reflectance, 0.48873));
}