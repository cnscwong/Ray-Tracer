#include <gtest/gtest.h>
#include "Sphere.h"
#include "LightAndShading.h"
#include "LightData.h"

TEST(SphereTest, NormalVectorTest){
    Sphere s;
    EXPECT_TRUE(s.computeNormal(Point(1, 0, 0)).isEqual(Vector(1, 0, 0)));
    EXPECT_TRUE(s.computeNormal(Point(0, 1, 0)).isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(s.computeNormal(Point(0, 0, 1)).isEqual(Vector(0, 0, 1)));
    EXPECT_TRUE(s.computeNormal(Point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)).isEqual(Vector(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3)));
    Vector temp = s.computeNormal(Point(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3));
    EXPECT_TRUE(temp.isEqual(temp.normalize()));
    s.setTransform(translationMatrix(0, 1, 0));
    EXPECT_TRUE(s.computeNormal(Point(0, 1.70711, -0.70711)).isEqual(Vector(0, 0.70711, -0.70711)));
    s.setTransform((scalingMatrix(1, 0.5, 1)*zRotationMatrix(PI/5)));
    EXPECT_TRUE(s.computeNormal(Point(0, sqrt(2)/2, -sqrt(2)/2)).isEqual(Vector(0, 0.97014, -0.24254)));
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

// Light       Eye         ||
TEST(LightingTest, EyeBetweenSurfaceAndLight){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Eye and normal vectors
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    // Light behind the eye
    LightSource light(Point(0, 0, -10), Colour(1, 1, 1));

    Colour result = computeLighting(m, position, light, eye, normal);
    EXPECT_TRUE(result.isEqual(Colour(1.9, 1.9, 1.9)));
}

//           Eye
// 
// 
// 
// Light                ||
// Eye is 45 degrees off of the normal vector
TEST(LightingTest, EyeAboveSurfaceAndLight){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Eye and normal vectors
    Vector eye(0, sqrt(2)/2, -sqrt(2)/2);
    Vector normal(0, 0, -1);
    // Light behind the eye
    LightSource light(Point(0, 0, -10), Colour(1, 1, 1));

    Colour result = computeLighting(m, position, light, eye, normal);
    EXPECT_TRUE(result.isEqual(Colour(1, 1, 1)));
}

//           Light
// 
// 
// 
//           Eye         ||
// Eye is opposite the surface, light is 45 degrees off the normal vector
TEST(LightingTest, LightAboveSurfaceAndEye){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Eye and normal vectors
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    // Light behind the eye
    LightSource light(Point(0, 10, -10), Colour(1, 1, 1));

    Colour result = computeLighting(m, position, light, eye, normal);
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
//             Eye
// light is 45 degrees off the normal vector, eye is in position
// where light is reflected into it(Also 45 degrees off normal vector)
TEST(LightingTest, LightAboveSurfaceEyeBelowSurface){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Eye and normal vectors
    Vector eye(0, -sqrt(2)/2, -sqrt(2)/2);
    Vector normal(0, 0, -1);
    // Light behind the eye
    LightSource light(Point(0, 10, -10), Colour(1, 1, 1));

    Colour result = computeLighting(m, position, light, eye, normal);
    EXPECT_TRUE(result.isEqual(Colour(1.6364, 1.6364, 1.6364)));
}

// Eye               ||       Light
// Light is behind surface, eye is opposite surface
TEST(LightingTest, LightBehindSurface){
    // Material of surface
    Material m;
    // Point the light is bouncing off of
    Point position(0, 0, 0);
    // Eye and normal vectors
    Vector eye(0, 0, -1);
    Vector normal(0, 0, -1);
    // Light behind the eye
    LightSource light(Point(0, 0, 10), Colour(1, 1, 1));

    Colour result = computeLighting(m, position, light, eye, normal);
    EXPECT_TRUE(result.isEqual(Colour(0.1, 0.1, 0.1)));
}

TEST(LightDataTest, PrepareLightDataTest){
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere s;
    Intersection i(4, s);
    LightData data = prepareLightData(i, r);
    EXPECT_EQ(data.time, 4);
    EXPECT_TRUE(data.object.isEqual(s));
    EXPECT_TRUE(data.point.isEqual(Point(0, 0, -1)));
    EXPECT_TRUE(data.eye.isEqual(Vector(0, 0, -1)));
    EXPECT_TRUE(data.normal.isEqual(Vector(0, 0, -1)));
    EXPECT_EQ(data.insideObject, false);

    r = Ray(Point(), Vector(0, 0, 1));
    i = Intersection(1, s);
    data = prepareLightData(i, r);
    EXPECT_TRUE(data.point.isEqual(Point(0, 0, 1)));
    EXPECT_TRUE(data.eye.isEqual(Vector(0, 0, -1)));
    EXPECT_TRUE(data.normal.isEqual(Vector(0, 0, -1)));
    EXPECT_EQ(data.insideObject, true);
}