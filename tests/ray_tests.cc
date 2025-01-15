#include <gtest/gtest.h>
#include "Ray.h"
#include "Sphere.h"
#include "Shape.h"
#include "common.h"
#include "Intersection.h"

TEST(RayTest, BasicTest){
    Ray r(Point(1, 2, 3), Vector(4, 5, 6));
    EXPECT_TRUE(r.getOrigin().isEqual(Point(1, 2, 3)));
    EXPECT_TRUE(r.getDirection().isEqual(Vector(4, 5, 6)));
}

TEST(RayTest, PositionTest){
    Ray r(Point(2, 3, 4), Vector(1, 0, 0));
    EXPECT_TRUE(r.computePosition(0).isEqual(Point(2, 3, 4)));
    EXPECT_TRUE(r.computePosition(1).isEqual(Point(3, 3, 4)));
    EXPECT_TRUE(r.computePosition(-1).isEqual(Point(1, 3, 4)));
    EXPECT_TRUE(r.computePosition(2.5).isEqual(Point(4.5, 3, 4)));
}

TEST(RayTest, IntersectionTest){
    // Ray intersects with the sphere at two points
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    Sphere* s = new Sphere;
    std::vector<Intersection> intersections = s->findIntersections(r);
    EXPECT_EQ(intersections.size(), 2);
    EXPECT_TRUE(floatIsEqual(intersections.at(0).getTime(), 4.0));
    EXPECT_TRUE(floatIsEqual(intersections.at(1).getTime(), 6.0));
    EXPECT_TRUE(intersections.at(0).getShape()->isEqual(s));
    EXPECT_TRUE(intersections.at(1).getShape()->isEqual(s));
    
    // Ray intersects the sphere at one point
    r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
    intersections = s->findIntersections(r);
    EXPECT_EQ(intersections.size(), 2);
    EXPECT_TRUE(floatIsEqual(intersections.at(0).getTime(), 5.0));
    EXPECT_TRUE(floatIsEqual(intersections.at(1).getTime(), 5.0));
    EXPECT_TRUE(intersections.at(0).getShape()->isEqual(s));
    EXPECT_TRUE(intersections.at(1).getShape()->isEqual(s));

    // Ray does not intersect sphere
    r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
    intersections = s->findIntersections(r);
    EXPECT_EQ(intersections.size(), 0);

    // Ray origin inside sphere, intersections should have one positive and one negative t value
    r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    intersections = s->findIntersections(r);
    EXPECT_EQ(intersections.size(), 2);
    EXPECT_TRUE(floatIsEqual(intersections.at(0).getTime(), -1.0));
    EXPECT_TRUE(floatIsEqual(intersections.at(1).getTime(), 1.0));
    EXPECT_TRUE(intersections.at(0).getShape()->isEqual(s));
    EXPECT_TRUE(intersections.at(1).getShape()->isEqual(s));

    // Sphere is behind the ray, both t values are negative
    r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
    intersections = s->findIntersections(r);
    EXPECT_EQ(intersections.size(), 2);
    EXPECT_TRUE(floatIsEqual(intersections.at(0).getTime(), -6.0));
    EXPECT_TRUE(floatIsEqual(intersections.at(1).getTime(), -4.0));
    EXPECT_TRUE(intersections.at(0).getShape()->isEqual(s));
    EXPECT_TRUE(intersections.at(1).getShape()->isEqual(s));
}

TEST(RayTest, TransformTest){
    Ray r(Point(1, 2, 3), Vector(0, 1, 0));
    Matrix m = translationMatrix(3, 4, 5);
    Ray r2 = r.transform(m);
    EXPECT_TRUE(r2.getOrigin().isEqual(Point(4, 6, 8)));
    EXPECT_TRUE(r2.getDirection().isEqual(Vector(0, 1, 0)));

    m = scalingMatrix(2, 3, 4);
    r2 = r.transform(m);
    EXPECT_TRUE(r2.getOrigin().isEqual(Point(2, 6, 12)));
    EXPECT_TRUE(r2.getDirection().isEqual(Vector(0, 3, 0)));
}

TEST(RayTest, TransformedSphereTest){
    Sphere s;
    EXPECT_TRUE(s.getTransform().isEqual(Matrix(4)));
    Matrix t = translationMatrix(2, 3, 4);
    s.setTransform(t);
    EXPECT_TRUE(s.getTransform().isEqual(t));

    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    s.setTransform(scalingMatrix(2, 2, 2));
    std::vector<Intersection> i = s.findIntersections(r);
    EXPECT_EQ(i.size(), 2);
    EXPECT_EQ(i.at(0).getTime(), 3);
    EXPECT_EQ(i.at(1).getTime(), 7);

    s.setTransform(translationMatrix(5, 0, 0));
    i = s.findIntersections(r);
    EXPECT_EQ(i.size(), 0);
}