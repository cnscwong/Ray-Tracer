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

    std::vector<Intersection> intersects = r.WorldIntersection(w);
    EXPECT_EQ(intersects.size(), 4);
    EXPECT_EQ(intersects.at(0).getTime(), 4);
    EXPECT_EQ(intersects.at(1).getTime(), 4.5);
    EXPECT_EQ(intersects.at(2).getTime(), 5.5);
    EXPECT_EQ(intersects.at(3).getTime(), 6);
}