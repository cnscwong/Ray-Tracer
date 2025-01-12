#include <gtest/gtest.h>
#include "World.h"
#include "Ray.h"

TEST(WorldTest, BasicTest){
    World w;
    LightSource l(Point(0, 0, 0), Colour(0, 0, 0));
    EXPECT_EQ(w.getObjects().size(), 0);
    EXPECT_TRUE(w.getLight().isEqual(l));

    w = defaultWorld();
    l = LightSource(Point(-10, 10, -10), Colour(1, 1, 1));

    Sphere s1;
    Material m;
    m.setColour(Colour(0.8, 1.0, 0.6));
    m.setDiffuse(0.7);
    m.setSpecular(0.2);
    s1.setMaterial(m);

    Sphere s2;
    s2.setTransform(scalingMatrix(0.5, 0.5, 0.5));
    EXPECT_EQ(w.getObjects().size(), 2);
    EXPECT_TRUE(w.getObjects().at(0).isEqual(s1));
    EXPECT_TRUE(w.getObjects().at(1).isEqual(s2));
    EXPECT_TRUE(w.getLight().isEqual(l));
}

TEST(WorldTest, IntersectWorldTest){
    World w = defaultWorld();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));

    std::vector<Intersection> intersects = w.RayIntersection(r);
    EXPECT_EQ(intersects.size(), 4);
    EXPECT_EQ(intersects.at(0).getTime(), 4);
    EXPECT_EQ(intersects.at(1).getTime(), 4.5);
    EXPECT_EQ(intersects.at(2).getTime(), 5.5);
    EXPECT_EQ(intersects.at(3).getTime(), 6);
}

TEST(WorldTest, ShadeHitTest){
    World w = defaultWorld();
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere s = w.getObjects().at(0);
    Intersection i(4, s);
    LightData data = prepareLightData(i, r);
    Colour c = w.shadeHit(data);
    EXPECT_TRUE(c.isEqual(Colour(0.38066, 0.47583, 0.2855)));

    w = defaultWorld();
    w.setLight(LightSource(Point(0, 0.25, 0), Colour(1, 1, 1)));
    r = Ray(Point(), Vector(0, 0, 1));
    s = w.getObjects().at(1);
    i = Intersection(0.5, s);
    data = prepareLightData(i, r);
    c = w.shadeHit(data);
    EXPECT_TRUE(c.isEqual(Colour(0.90498, 0.90498, 0.90498)));
}

TEST(WorldTest, ColourAtHitTest){
    World w = defaultWorld();
    Ray r(Point(0, 0, -5), Vector(0, 1, 0));
    Colour c = w.colourAtHit(r);
    EXPECT_TRUE(c.isEqual(Colour()));

    r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    c = w.colourAtHit(r);
    EXPECT_TRUE(c.isEqual(Colour(0.38066, 0.47583, 0.2855)));

    std::vector<Sphere> objs = w.getObjects();
    Material m = objs.at(0).getMaterial();
    m.setAmbient(1);
    objs.at(0).setMaterial(m);
    m = objs.at(1).getMaterial();
    m.setAmbient(1);
    objs.at(1).setMaterial(m);
    w.setObjects(objs);
    r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
    c = w.colourAtHit(r);
    EXPECT_TRUE(c.isEqual(w.getObjects().at(1).getMaterial().getColour()));
}

TEST(WorldTest, ViewTransformationTest){
    // The world does not require any scaling, rotating, or translation so the result is the identity matrix
    Point eyePosition(0, 0, 0);
    Point to(0, 0, -1);
    Vector up(0, 1, 0);
    EXPECT_TRUE(viewTransformationMatrix(eyePosition, to, up).isEqual(Matrix(4)));

    // Since reflection is the same as scaling by negative values, turning the camera to look at the 
    // positive z direction will result in the scalingMatrix(-1, 1, -1)
    to = Point(0, 0, 1);
    EXPECT_TRUE(viewTransformationMatrix(eyePosition, to, up).isEqual(scalingMatrix(-1, 1, -1)));

    // The resulting matrix moves the world back 8 units
    eyePosition = Point(0, 0, 8);
    to = Point();
    EXPECT_TRUE(viewTransformationMatrix(eyePosition, to, up).isEqual(translationMatrix(0, 0, -8)));

    // Matrix that is a combination of shearing, scaling, and translation
    eyePosition = Point(1, 3, 2);
    to = Point(4, -2, 8);
    up = Point(1, 1, 0);
    Matrix m(4);
    
    m.setElement(0, 0, -0.50709);
    m.setElement(0, 1, 0.50709);
    m.setElement(0, 2, 0.67612);
    m.setElement(0, 3, -2.36643);

    m.setElement(1, 0, 0.76772);
    m.setElement(1, 1, 0.60609);
    m.setElement(1, 2, 0.12122);
    m.setElement(1, 3, -2.82843);

    m.setElement(2, 0, -0.35857);
    m.setElement(2, 1, 0.59761);
    m.setElement(2, 2, -0.71714);
    EXPECT_TRUE(viewTransformationMatrix(eyePosition, to, up).isEqual(m));
}