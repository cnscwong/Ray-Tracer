#include <gtest/gtest.h>
#include "Ray.h"
#include "Shape.h"
#include "common.h"
#include "Intersection.h"

// Testing the intersections function
TEST(IntersectionTest, PackingTest){
    Shape* s = new Sphere;
    Intersection i(3.5, s);
    EXPECT_EQ(i.getTime(), 3.5);
    EXPECT_TRUE(i.getShape()->isEqual(s));
    Intersection i1(1, s);
    Intersection i2(2, s);
    std::vector<Intersection> intersects = intersections({i1, i2});
    EXPECT_EQ(intersects.size(), 2);
    EXPECT_EQ(intersects.at(0).getTime(), 1);
    EXPECT_EQ(intersects.at(1).getTime(), 2);
    delete s;
}

// Testing the hits function
TEST(IntersectionTest, FindingHits){
    Sphere* s = new Sphere;
    Intersection i1(1, s);
    Intersection i2(2, s);
    std::vector<Intersection> intersects = intersections({i2, i1});
    int ind = hit(intersects);
    EXPECT_EQ(ind, 1);

    i1 = Intersection(-1, s);
    i2 = Intersection(1, s);
    intersects = intersections({i2, i1});
    ind = hit(intersects);
    EXPECT_EQ(ind, 0);

    i1 = Intersection(-2, s);
    i2 = Intersection(-1, s);
    intersects = intersections({i2, i1});
    ind = hit(intersects);
    EXPECT_EQ(ind, -1);

    i1 = Intersection(5, s);
    i2 = Intersection(7, s);
    Intersection i3(-3, s);
    Intersection i4(2, s);
    intersects = intersections({i1, i2, i3, i4});
    ind = hit(intersects);
    EXPECT_EQ(ind, 3);
    delete s;
}