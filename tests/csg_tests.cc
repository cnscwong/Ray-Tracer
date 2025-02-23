#include <gtest/gtest.h>
#include "CSG.h"

TEST(CSGTest, BasicTest){
    Sphere* l = new Sphere;
    Cube* r = new Cube;
    CSG* csg = new CSG(UNION, l, r);

    EXPECT_EQ(csg->getOp(), UNION);
    EXPECT_TRUE(csg->getLeft()->isEqual(l));
    EXPECT_TRUE(csg->getRight()->isEqual(r));
    EXPECT_TRUE(l->getParent()->isEqual(csg));
    EXPECT_TRUE(r->getParent()->isEqual(csg));

    delete l;
    delete r;
    delete csg;
    l = nullptr;
    r = nullptr;
    csg = nullptr;
}

TEST(CSGTest, validIntersectionUnionTest){
    Shape* l = new Shape;
    Shape* r = new Shape;
    CSG* csg = new CSG(UNION, l, r);

    EXPECT_FALSE(csg->validIntersection(true, true, true));
    EXPECT_TRUE(csg->validIntersection(true, true, false));
    EXPECT_FALSE(csg->validIntersection(true, false, true));
    EXPECT_TRUE(csg->validIntersection(true, false, false));
    EXPECT_FALSE(csg->validIntersection(false, true, true));
    EXPECT_FALSE(csg->validIntersection(false, true, false));
    EXPECT_TRUE(csg->validIntersection(false, false, true));
    EXPECT_TRUE(csg->validIntersection(false, false, false));
}

TEST(CSGTest, validIntersectionIntersectionTest){
    Shape* l = new Shape;
    Shape* r = new Shape;
    CSG* csg = new CSG(INTERSECTION, l, r);

    EXPECT_TRUE(csg->validIntersection(true, true, true));
    EXPECT_FALSE(csg->validIntersection(true, true, false));
    EXPECT_TRUE(csg->validIntersection(true, false, true));
    EXPECT_FALSE(csg->validIntersection(true, false, false));
    EXPECT_TRUE(csg->validIntersection(false, true, true));
    EXPECT_TRUE(csg->validIntersection(false, true, false));
    EXPECT_FALSE(csg->validIntersection(false, false, true));
    EXPECT_FALSE(csg->validIntersection(false, false, false));
}

TEST(CSGTest, validIntersectionDifferenceTest){
    Shape* l = new Shape;
    Shape* r = new Shape;
    CSG* csg = new CSG(DIFFERENCE, l, r);

    EXPECT_FALSE(csg->validIntersection(true, true, true));
    EXPECT_TRUE(csg->validIntersection(true, true, false));
    EXPECT_FALSE(csg->validIntersection(true, false, true));
    EXPECT_TRUE(csg->validIntersection(true, false, false));
    EXPECT_TRUE(csg->validIntersection(false, true, true));
    EXPECT_TRUE(csg->validIntersection(false, true, false));
    EXPECT_FALSE(csg->validIntersection(false, false, true));
    EXPECT_FALSE(csg->validIntersection(false, false, false));
}

TEST(CSGTest, filterIntersectionsUnionTest){
    Shape* l = new Shape;
    Shape* r = new Shape;
    CSG* csg = new CSG(UNION, l, r);
    std::vector<Intersection> intersects({Intersection(1, l), Intersection(2, r), Intersection(3, l), Intersection(4, r)});

    std::vector<Intersection> result = csg->filterIntersections(intersects);

    EXPECT_EQ(result.size(), 2);
    EXPECT_TRUE(result.at(0).isEqual(intersects.at(0)));
    EXPECT_TRUE(result.at(1).isEqual(intersects.at(3)));
}

TEST(CSGTest, filterIntersectionsIntersectionTest){
    Shape* l = new Shape;
    Shape* r = new Shape;
    CSG* csg = new CSG(INTERSECTION, l, r);
    std::vector<Intersection> intersects({Intersection(1, l), Intersection(2, r), Intersection(3, l), Intersection(4, r)});

    std::vector<Intersection> result = csg->filterIntersections(intersects);

    EXPECT_EQ(result.size(), 2);
    EXPECT_TRUE(result.at(0).isEqual(intersects.at(1)));
    EXPECT_TRUE(result.at(1).isEqual(intersects.at(2)));
}

TEST(CSGTest, filterIntersectionsDifferenceTest){
    Shape* l = new Shape;
    Shape* r = new Shape;
    CSG* csg = new CSG(DIFFERENCE, l, r);
    std::vector<Intersection> intersects({Intersection(1, l), Intersection(2, r), Intersection(3, l), Intersection(4, r)});

    std::vector<Intersection> result = csg->filterIntersections(intersects);

    EXPECT_EQ(result.size(), 2);
    EXPECT_TRUE(result.at(0).isEqual(intersects.at(0)));
    EXPECT_TRUE(result.at(1).isEqual(intersects.at(1)));
}

TEST(CSGTest, childIntersectionsRayMissesTest){
    Sphere* s = new Sphere;
    Cube* c = new Cube;
    CSG* csg = new CSG(UNION, s, c);
    Ray r(Point(0, 2, -5), Vector(0, 0, 1));

    std::vector<Intersection> result = csg->childIntersections(r);

    EXPECT_EQ(result.size(), 0);
}

TEST(CSGTest, childIntersectionsRayHitsCSGTest){
    Sphere* s1 = new Sphere;
    Sphere* s2 = new Sphere;
    s2->setTransform(translationMatrix(0, 0, 0.5));
    CSG* csg = new CSG(UNION, s1, s2);
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));

    std::vector<Intersection> result = csg->childIntersections(r);

    EXPECT_EQ(result.size(), 2);
    EXPECT_TRUE(floatIsEqual(result.at(0).getTime(), 4));
    EXPECT_TRUE(result.at(0).getShape()->isEqual(s1));
    EXPECT_TRUE(floatIsEqual(result.at(1).getTime(), 6.5));
    EXPECT_TRUE(result.at(1).getShape()->isEqual(s2));
}