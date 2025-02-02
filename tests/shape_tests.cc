#include <gtest/gtest.h>
#include "Shape.h"
#include "Matrix.h"
#include "LightAndShading.h"
#include <vector>
#include "Intersection.h"
#include "Ray.h"

TEST(ShapeTest, BasicTest){
    Shape s;
    EXPECT_TRUE(s.getTransform().isEqual(Matrix(4)));
    EXPECT_TRUE(s.getMaterial().isEqual(Material()));
    s.setTransform(translationMatrix(2, 3, 4));
    EXPECT_TRUE(s.getTransform().isEqual(translationMatrix(2, 3, 4)));
    Material m;
    m.ambient = 1;
    s.setMaterial(m);
    EXPECT_TRUE(s.getMaterial().isEqual(m));
}

TEST(PlaneChildNormalTest, AllNormalVectorsAreTheSame){
    Plane* p = new Plane;
    Vector n1 = p->childNormal(Point());
    Vector n2 = p->childNormal(Point(10, 0, -10));
    Vector n3 = p->childNormal(Point(-5, 0, 150));

    EXPECT_TRUE(n1.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(n2.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(n3.isEqual(Vector(0, 1, 0)));
}

TEST(PlaneChildIntersectionsTest, NoIntersectionsWithRayParallelToPlaneButNotOnThePlane){
    Plane* p = new Plane;
    Ray r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
    std::vector<Intersection> intersects = p->childIntersections(r);
    EXPECT_EQ(intersects.size(), 0);
}

TEST(PlaneChildIntersectionsTest, NoIntersectionsWithCoplanarRay){
    Plane* p = new Plane;
    Ray r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
    std::vector<Intersection> intersects = p->childIntersections(r);
    EXPECT_EQ(intersects.size(), 0);
}

TEST(PlaneChildIntersectionsTest, RayIntersectsPlaneFromBothSides){
    Plane* p = new Plane;
    Ray r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
    std::vector<Intersection> intersects = p->childIntersections(r);
    EXPECT_EQ(intersects.size(), 1);
    EXPECT_EQ(intersects.at(0).getTime(), 1);
    EXPECT_TRUE(intersects.at(0).getShape()->isEqual(p));

    r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
    intersects = p->childIntersections(r);
    EXPECT_EQ(intersects.size(), 1);
    EXPECT_EQ(intersects.at(0).getTime(), 1);
    EXPECT_TRUE(intersects.at(0).getShape()->isEqual(p));
}

TEST(GlassSphereTest, SphereIsTransparentAndRefractive){
    Sphere* s = glassSphere();
    EXPECT_TRUE(s->getTransform().isEqual(Matrix(4)));
    EXPECT_EQ(s->getMaterial().transparency, 1);
    EXPECT_EQ(s->getMaterial().refractiveIndex, 1.5);
}