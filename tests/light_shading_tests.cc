#include <gtest/gtest.h>
#include "Sphere.h"
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
    EXPECT_TRUE(m.getColour().isEqual(Colour(1, 1, 1)));
    EXPECT_TRUE(floatIsEqual(m.getAmbient(), 0.1));
    EXPECT_TRUE(floatIsEqual(m.getDiffuse(), 0.9));
    EXPECT_TRUE(floatIsEqual(m.getSpecular(), 0.9));
    EXPECT_TRUE(floatIsEqual(m.getShininess(), 200));

    Sphere s;
    m = s.getMaterial();
    EXPECT_TRUE(m.isEqual(Material()));

    m.setAmbient(1);
    EXPECT_TRUE(floatIsEqual(m.getAmbient(), 1));
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

    Colour result = computeLighting(m, position, light, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(1.9, 1.9, 1.9)));
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

    Colour result = computeLighting(m, position, light, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(1, 1, 1)));
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

    Colour result = computeLighting(m, position, light, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(0.7364, 0.7364, 0.7364)));
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

    Colour result = computeLighting(m, position, light, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(1.6364, 1.6364, 1.6364)));
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

    Colour result = computeLighting(m, position, light, camera, normal, false);
    EXPECT_TRUE(result.isEqual(Colour(0.1, 0.1, 0.1)));
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
}

// Test for the overPoint variable in the LightData 
// Checks that it is a little bit above the object
TEST(WorldTest, OverpointTest){
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(0, 0, 1));
    Intersection i(5, s);
    LightData data = prepareLightData(i, r);
    EXPECT_TRUE(data.overPoint.z < -EPSILON/2);
    EXPECT_TRUE(data.point.z > data.overPoint.z);
}