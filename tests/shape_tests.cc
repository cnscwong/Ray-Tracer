#include <gtest/gtest.h>
#include "Shape.h"
#include "Matrix.h"
#include "LightAndShading.h"
#include <vector>
#include "Intersection.h"
#include "Ray.h"
#include "Group.h"

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
    EXPECT_EQ(s.getParent(), nullptr);
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

TEST(CubeChildIntersectionsTest, RayIntersectsCube){
    Cube* c = new Cube;
    Ray rayFromPosXDirection(Point(5, 0.5, 0), Vector(-1, 0, 0));
    Ray rayFromNegXDirection(Point(-5, 0.5, 0), Vector(1, 0, 0));
    Ray rayFromPosYDirection(Point(0.5, 5, 0), Vector(0, -1, 0));
    Ray rayFromNegYDirection(Point(0.5, -5, 0), Vector(0, 1, 0));
    Ray rayFromPosZDirection(Point(0.5, 0, 5), Vector(0, 0, -1));
    Ray rayFromNegZDirection(Point(0.5, 0, -5), Vector(0, 0, 1));
    Ray rayFromInsideCube(Point(0, 0.5, 0), Vector(0, 0, 1));

    std::vector<Intersection> result1 = c->childIntersections(rayFromPosXDirection);
    std::vector<Intersection> result2 = c->childIntersections(rayFromNegXDirection);
    std::vector<Intersection> result3 = c->childIntersections(rayFromPosYDirection);
    std::vector<Intersection> result4 = c->childIntersections(rayFromNegYDirection);
    std::vector<Intersection> result5 = c->childIntersections(rayFromPosZDirection);
    std::vector<Intersection> result6 = c->childIntersections(rayFromNegZDirection);
    std::vector<Intersection> result7 = c->childIntersections(rayFromInsideCube);

    EXPECT_EQ(result1.size(), 2);
    EXPECT_EQ(result2.size(), 2);
    EXPECT_EQ(result3.size(), 2);
    EXPECT_EQ(result4.size(), 2);
    EXPECT_EQ(result5.size(), 2);
    EXPECT_EQ(result6.size(), 2);
    EXPECT_EQ(result7.size(), 2);
    EXPECT_TRUE(floatIsEqual(result1.at(0).getTime(), 4));
    EXPECT_TRUE(floatIsEqual(result1.at(1).getTime(), 6));
    EXPECT_TRUE(floatIsEqual(result2.at(0).getTime(), 4));
    EXPECT_TRUE(floatIsEqual(result2.at(1).getTime(), 6));
    EXPECT_TRUE(floatIsEqual(result3.at(0).getTime(), 4));
    EXPECT_TRUE(floatIsEqual(result3.at(1).getTime(), 6));
    EXPECT_TRUE(floatIsEqual(result4.at(0).getTime(), 4));
    EXPECT_TRUE(floatIsEqual(result4.at(1).getTime(), 6));
    EXPECT_TRUE(floatIsEqual(result5.at(0).getTime(), 4));
    EXPECT_TRUE(floatIsEqual(result5.at(1).getTime(), 6));
    EXPECT_TRUE(floatIsEqual(result6.at(0).getTime(), 4));
    EXPECT_TRUE(floatIsEqual(result6.at(1).getTime(), 6));
    EXPECT_TRUE(floatIsEqual(result7.at(0).getTime(), -1));
    EXPECT_TRUE(floatIsEqual(result7.at(1).getTime(), 1));
}

TEST(CubeChildIntersectionsTest, RayMissesCube){
    Cube* c = new Cube;
    Ray r(Point(2, 2, 0), Vector(-1, 0, 0));

    std::vector<Intersection> result = c->childIntersections(r);

    EXPECT_EQ(result.size(), 0);
}

TEST(CubeChildNormalTest, NormalIsCorrectForDifferentSidesOfTheCube){
    Cube* c = new Cube;
    Point p1(1, 0.5, -0.8);
    Point p2(-1, -0.2, 0.9);
    Point p3(-0.4, 1, -0.1);
    Point p4(0.3, -1, -0.7);
    Point p5(-0.6, 0.3, 1);
    Point p6(0.4, 0.4, -1);
    Point p7(1, 1, 1);
    Point p8(-1, -1, -1);

    Vector result1 = c->childNormal(p1);
    Vector result2 = c->childNormal(p2);
    Vector result3 = c->childNormal(p3);
    Vector result4 = c->childNormal(p4);
    Vector result5 = c->childNormal(p5);
    Vector result6 = c->childNormal(p6);
    Vector result7 = c->childNormal(p7);
    Vector result8 = c->childNormal(p8);

    EXPECT_TRUE(result1.isEqual(Vector(1, 0, 0)));
    EXPECT_TRUE(result2.isEqual(Vector(-1, 0, 0)));
    EXPECT_TRUE(result3.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(result4.isEqual(Vector(0, -1, 0)));
    EXPECT_TRUE(result5.isEqual(Vector(0, 0, 1)));
    EXPECT_TRUE(result6.isEqual(Vector(0, 0, -1)));
    EXPECT_TRUE(result7.isEqual(Vector(1, 0, 0)));
    EXPECT_TRUE(result8.isEqual(Vector(-1, 0, 0)));
}

TEST(CylinderTest, BasicTest){
    Cylinder* c = new Cylinder;

    EXPECT_EQ(c->getMaxH(), INFINITY);
    EXPECT_EQ(c->getMinH(), -INFINITY);
    EXPECT_EQ(c->getClosed(), false);
}

TEST(CylinderChildIntersectionsTest, RayMissesCylinder){
    Cylinder* c = new Cylinder;
    Ray ray1(Point(1, 0, 0), Vector(0, 1, 0));
    Ray ray2(Point(0, 0, 0), Vector(0, 1, 0));
    Ray ray3(Point(0, 0, -5), Vector(1, 1, 1));

    std::vector<Intersection> result1 = c->childIntersections(ray1);
    std::vector<Intersection> result2 = c->childIntersections(ray2);
    std::vector<Intersection> result3 = c->childIntersections(ray3);

    EXPECT_EQ(result1.size(), 0);
    EXPECT_EQ(result2.size(), 0);
    EXPECT_EQ(result3.size(), 0);
}

TEST(CylinderChildIntersectionsTest, RayIntersectsCylinder){
    Cylinder* c = new Cylinder;
    Ray ray1(Point(1, 0, -5), Vector(0, 0, 1));
    Ray ray2(Point(0, 0, -5), Vector(0, 0, 1));
    Ray ray3(Point(0.5, 0, -5), Vector(0.1, 1, 1));

    std::vector<Intersection> result1 = c->childIntersections(ray1);
    std::vector<Intersection> result2 = c->childIntersections(ray2);
    std::vector<Intersection> result3 = c->childIntersections(ray3);

    EXPECT_EQ(result1.size(), 2);
    EXPECT_EQ(result2.size(), 2);
    EXPECT_EQ(result3.size(), 2);
    EXPECT_EQ(result1.at(0).getTime(), 5);
    EXPECT_EQ(result1.at(1).getTime(), 5);
    EXPECT_EQ(result2.at(0).getTime(), 4);
    EXPECT_EQ(result2.at(1).getTime(), 6);
    EXPECT_TRUE(floatIsEqual(result3.at(0).getTime(), 4.80199));
    EXPECT_TRUE(floatIsEqual(result3.at(1).getTime(), 4.99998));
}

TEST(CylinderChildIntersectionsTest, RayIntersectsCylinderWithHeightConstraints){
    Cylinder* c = new Cylinder;
    c->setMaxH(2);
    c->setMinH(1);
    Ray ray1(Point(0, 1.5, 0), Vector(0.1, 1, 0));
    Ray ray2(Point(0, 3, -5), Vector(0, 0, 1));
    Ray ray3(Point(0, 0, -5), Vector(0, 0, 1));
    Ray ray4(Point(0, 2, -5), Vector(0, 0, 1));
    Ray ray5(Point(0, 1, -5), Vector(0, 0, 1));
    Ray ray6(Point(0, 1.5, -2), Vector(0, 0, 1));

    std::vector<Intersection> result1 = c->childIntersections(ray1);
    std::vector<Intersection> result2 = c->childIntersections(ray2);
    std::vector<Intersection> result3 = c->childIntersections(ray3);
    std::vector<Intersection> result4 = c->childIntersections(ray4);
    std::vector<Intersection> result5 = c->childIntersections(ray5);
    std::vector<Intersection> result6 = c->childIntersections(ray6);

    EXPECT_EQ(result1.size(), 0);
    EXPECT_EQ(result2.size(), 0);
    EXPECT_EQ(result3.size(), 0);
    EXPECT_EQ(result4.size(), 0);
    EXPECT_EQ(result5.size(), 0);
    EXPECT_EQ(result6.size(), 2);
    EXPECT_EQ(result6.at(0).getTime(), 1);
    EXPECT_EQ(result6.at(1).getTime(), 3);
}

TEST(CylinderChildIntersectionsTest, RayIntersectsWithCylinderCaps){
    Cylinder* c = new Cylinder;
    c->setMaxH(2);
    c->setMinH(1);
    c->setClosed(true);
    Ray ray1(Point(0, 3, 0), Vector(0, -1, 0));
    Ray ray2(Point(0, 3, -2), Vector(0, -1, 2));
    Ray ray3(Point(0, 4, -2), Vector(0, -1, 1));
    Ray ray4(Point(0, 0, -2), Vector(0, 1, 2));
    Ray ray5(Point(0, -1, -2), Vector(0, 1, 1));

    std::vector<Intersection> result1 = c->childIntersections(ray1);
    std::vector<Intersection> result2 = c->childIntersections(ray2);
    std::vector<Intersection> result3 = c->childIntersections(ray3);
    std::vector<Intersection> result4 = c->childIntersections(ray4);
    std::vector<Intersection> result5 = c->childIntersections(ray5);

    EXPECT_EQ(result1.size(), 2);
    EXPECT_EQ(result2.size(), 2);
    EXPECT_EQ(result3.size(), 2);
    EXPECT_EQ(result4.size(), 2);
    EXPECT_EQ(result5.size(), 2);
    EXPECT_EQ(result1.at(0).getTime(), 2);
    EXPECT_EQ(result1.at(1).getTime(), 1);
    EXPECT_EQ(result2.at(0).getTime(), 1.5);
    EXPECT_EQ(result2.at(1).getTime(), 1);
    EXPECT_EQ(result3.at(0).getTime(), 3);
    EXPECT_EQ(result3.at(1).getTime(), 2);
    EXPECT_EQ(result4.at(0).getTime(), 1.5);
    EXPECT_EQ(result4.at(1).getTime(), 1);
    EXPECT_EQ(result5.at(0).getTime(), 2);
    EXPECT_EQ(result5.at(1).getTime(), 3);
}

TEST(CylinderChildNormalTest, NormalIsCorrectOnCylinderWalls){
    Cylinder* c = new Cylinder;
    Point p1(1, 0, 0);
    Point p2(0, 5, -1);
    Point p3(0, -2, 1);
    Point p4(-1, 1, 0);

    Vector result1 = c->childNormal(p1);
    Vector result2 = c->childNormal(p2);
    Vector result3 = c->childNormal(p3);
    Vector result4 = c->childNormal(p4);

    EXPECT_TRUE(result1.isEqual(Vector(1, 0, 0)));
    EXPECT_TRUE(result2.isEqual(Vector(0, 0, -1)));
    EXPECT_TRUE(result3.isEqual(Vector(0, 0, 1)));
    EXPECT_TRUE(result4.isEqual(Vector(-1, 0, 0)));
}

TEST(CylinderChildNormalTest, NormalIsCorrectOnCylinderCaps){
    Cylinder* c = new Cylinder;
    c->setMaxH(2);
    c->setMinH(1);
    c->setClosed(true);
    Point p1(0, 1, 0);
    Point p2(0.5, 1, 0);
    Point p3(0, 1, 0.5);
    Point p4(0, 2, 0);
    Point p5(0.5, 2, 0);
    Point p6(0, 2, 0.5);

    Vector result1 = c->childNormal(p1);
    Vector result2 = c->childNormal(p2);
    Vector result3 = c->childNormal(p3);
    Vector result4 = c->childNormal(p4);
    Vector result5 = c->childNormal(p5);
    Vector result6 = c->childNormal(p6);

    EXPECT_TRUE(result1.isEqual(Vector(0, -1, 0)));
    EXPECT_TRUE(result2.isEqual(Vector(0, -1, 0)));
    EXPECT_TRUE(result3.isEqual(Vector(0, -1, 0)));
    EXPECT_TRUE(result4.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(result5.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(result6.isEqual(Vector(0, 1, 0)));
}

TEST(ConeTest, BasicTest){
    Cone* c = new Cone;

    EXPECT_EQ(c->getMaxH(), INFINITY);
    EXPECT_EQ(c->getMinH(), -INFINITY);
    EXPECT_EQ(c->getClosed(), false);
}

TEST(ConeChildIntersectionsTest, RayMissesCone){
    Cone* c = new Cone;
    Ray ray1(Point(1, 0, 0), Vector(0, 0, 1));

    std::vector<Intersection> result1 = c->childIntersections(ray1);

    EXPECT_EQ(result1.size(), 0);
}

TEST(ConeChildIntersectionsTest, RayIntersectsCone){
    Cone* c = new Cone;
    Ray ray1(Point(0, 0, -5), Vector(0, 0, 1));
    Ray ray2(Point(0, 0, -5), Vector(1, 1, 1).normalize());
    Ray ray3(Point(1, 1, -5), Vector(-0.5, -1, 1).normalize());

    std::vector<Intersection> result1 = c->childIntersections(ray1);
    std::vector<Intersection> result2 = c->childIntersections(ray2);
    std::vector<Intersection> result3 = c->childIntersections(ray3);

    EXPECT_EQ(result1.size(), 2);
    EXPECT_EQ(result2.size(), 2);
    EXPECT_EQ(result3.size(), 2);
    EXPECT_EQ(result1.at(0).getTime(), 5);
    EXPECT_EQ(result1.at(1).getTime(), 5);
    EXPECT_TRUE(floatIsEqual(result2.at(0).getTime(), 8.66025));
    EXPECT_TRUE(floatIsEqual(result2.at(1).getTime(), 8.66025));
    EXPECT_TRUE(floatIsEqual(result3.at(0).getTime(), 4.55006));
    EXPECT_TRUE(floatIsEqual(result3.at(1).getTime(), 49.44994));
}

TEST(ConeChildIntersectionsTest, ParallelRayIntersectsCone){
    Cone* c = new Cone;
    Ray r(Point(0, 0, -1), Vector(0, 1, 1).normalize());

    std::vector<Intersection> result = c->childIntersections(r);

    EXPECT_EQ(result.size(), 1);
    EXPECT_TRUE(floatIsEqual(result.at(0).getTime(), 0.35355));
}

TEST(ConeChildIntersectionsTest, RayIntersectsWithConeCaps){
    Cone* c = new Cone;
    c->setMaxH(0.5);
    c->setMinH(-0.5);
    c->setClosed(true);
    Ray ray1(Point(0, 0, -5), Vector(0, 1, 0));
    Ray ray2(Point(0, 0, -0.25), Vector(0, 1, 1).normalize());
    Ray ray3(Point(0, 0, -0.25), Vector(0, 1, 0));

    std::vector<Intersection> result1 = c->childIntersections(ray1);
    std::vector<Intersection> result2 = c->childIntersections(ray2);
    std::vector<Intersection> result3 = c->childIntersections(ray3);

    EXPECT_EQ(result1.size(), 0);
    EXPECT_EQ(result2.size(), 2);
    EXPECT_EQ(result3.size(), 4);
}

TEST(ConeChildNormalTest, NormalIsCorrectOnConeWalls){
    Cone* c = new Cone;
    Point p1(0, 0, 0);
    Point p2(1, 1, 1);
    Point p3(-1, -1, 0);

    Vector result1 = c->childNormal(p1);
    Vector result2 = c->childNormal(p2);
    Vector result3 = c->childNormal(p3);

    EXPECT_TRUE(result1.isEqual(Vector(0, 0, 0)));
    EXPECT_TRUE(result2.isEqual(Vector(1, -sqrt(2), 1)));
    EXPECT_TRUE(result3.isEqual(Vector(-1, 1, 0)));
}

TEST(ConeChildNormalTest, NormalIsCorrectOnConeCaps){
    Cone* c = new Cone;
    c->setMaxH(4);
    c->setMinH(-8);
    c->setClosed(true);
    Point p1(0, 4, 0);
    Point p2(1.5, 4, 1.5);
    Point p3(-1, -8, 3);
    Point p4(6, -8, 4);

    Vector result1 = c->childNormal(p1);
    Vector result2 = c->childNormal(p2);
    Vector result3 = c->childNormal(p3);
    Vector result4 = c->childNormal(p4);

    EXPECT_TRUE(result1.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(result2.isEqual(Vector(0, 1, 0)));
    EXPECT_TRUE(result3.isEqual(Vector(0, -1, 0)));
    EXPECT_TRUE(result4.isEqual(Vector(0, -1, 0)));
}

TEST(GroupTest, BasicTest){
    Group* g = new Group;

    EXPECT_TRUE(g->getTransform().isEqual(Matrix(4)));
    EXPECT_EQ(g->getShapes().size(), 0);
}

TEST(Group_appendShapeTest, GroupContainsShapeAndShapeHasParentGroupTest){
    Group* g = new Group;
    Shape* s = new Shape;

    g->appendShape(s);

    EXPECT_EQ(g->getShapes().size(), 1);
    EXPECT_TRUE(g->getShapes().at(0)->isEqual(s));
    EXPECT_EQ(s->getParent(), g);
}

TEST(Group_childIntersectionsTest, RayMissesGroup){
    Group* g = new Group;
    Ray r(Point(), Vector(0, 0, 1));

    std::vector<Intersection> result = g->childIntersections(r);

    EXPECT_EQ(result.size(), 0);
}

TEST(Group_childIntersectionsTest, RayHitsGroup){
    Group* g = new Group;
    Sphere* s1 = new Sphere;
    Sphere* s2 = new Sphere;
    s2->setTransform(translationMatrix(0, 0, -3));
    Sphere* s3 = new Sphere;
    s3->setTransform(translationMatrix(5, 0, 0));
    g->appendShape(s1);
    g->appendShape(s2);
    g->appendShape(s3);
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));

    std::vector<Intersection> result = g->childIntersections(r);

    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result.at(0).getShape(), s2);
    EXPECT_EQ(result.at(1).getShape(), s2);
    EXPECT_EQ(result.at(2).getShape(), s1);
    EXPECT_EQ(result.at(3).getShape(), s1);
}

TEST(Group_findIntersectionsTest, GroupTransformationAppliedToChildShapes){
    Group* g = new Group;
    g->setTransform(scalingMatrix(2, 2, 2));
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(5, 0, 0));
    g->appendShape(s);
    Ray r(Point(10, 0, -10), Vector(0, 0, 1));

    // Only intersects s if the group transformation is applied
    std::vector<Intersection> result = g->findIntersections(r);

    EXPECT_EQ(result.size(), 2);
}

TEST(Shape_WorldToObjectTest, PointConvertedToObjectSpaceInNestedGroup){
    Group* g1 = new Group;
    g1->setTransform(yRotationMatrix(PI/2));
    Group* g2 = new Group;
    g2->setTransform(scalingMatrix(2, 2, 2));
    g1->appendShape(g2);
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(5, 0, 0));
    g2->appendShape(s);

    Point p = s->worldToObject(Point(-2, 0, -10));

    EXPECT_TRUE(p.isEqual(Point(0, 0, -1)));
}

TEST(Shape_NormalToWorldTest, NormalConvertedToObjectSpaceInNestedGroup){
    Group* g1 = new Group;
    g1->setTransform(yRotationMatrix(PI/2));
    Group* g2 = new Group;
    g2->setTransform(scalingMatrix(1, 2, 3));
    g1->appendShape(g2);
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(5, 0, 0));
    g2->appendShape(s);

    Vector n = s->normalToWorld(Vector(sqrt(3)/3, sqrt(3)/3, sqrt(3)/3));

    EXPECT_TRUE(n.isEqual(Vector(0.2857, 0.4286, -0.8571)));
}

TEST(Shape_ComputeNormalTest, NormalCorrectForChildShapeInNestedGroup){
    Group* g1 = new Group;
    g1->setTransform(yRotationMatrix(PI/2));
    Group* g2 = new Group;
    g2->setTransform(scalingMatrix(1, 2, 3));
    g1->appendShape(g2);
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(5, 0, 0));
    g2->appendShape(s);

    Vector n = s->computeNormal(Point(1.7321, 1.1547, -5.5774));

    EXPECT_TRUE(n.isEqual(Vector(0.2857, 0.4286, -0.8571)));
}

TEST(Triangle_ConstructorTest, BasicTest){
    Point p1(0, 1, 0);
    Point p2(-1, 0, 0);
    Point p3(1, 0, 0);
    Triangle* t = new Triangle(p1, p2, p3);

    EXPECT_TRUE(t->getP1().isEqual(p1));
    EXPECT_TRUE(t->getP2().isEqual(p2));
    EXPECT_TRUE(t->getP3().isEqual(p3));
    EXPECT_TRUE(t->getE1().isEqual(Vector(-1, -1, 0)));
    EXPECT_TRUE(t->getE2().isEqual(Vector(1, -1, 0)));
    EXPECT_TRUE(t->getNormal().isEqual(Vector(0, 0, -1)));
}

TEST(Triangle_ComputeNormalTest, NormalCorrectOnTriangle){
    Triangle* t = new Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    
    Vector n1 = t->childNormal(Point(0, 0.5, 0));
    Vector n2 = t->childNormal(Point(-0.5, 0.75, 0));
    Vector n3 = t->childNormal(Point(0.5, 0.25, 0));

    EXPECT_TRUE(n1.isEqual(t->getNormal()));
    EXPECT_TRUE(n2.isEqual(t->getNormal()));
    EXPECT_TRUE(n3.isEqual(t->getNormal()));
}

TEST(Triangle_ChildIntersectionTest, RayParallelToTriangle){
    Triangle* t = new Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(0, -1, -2), Vector(0, 1, 0));

    std::vector<Intersection> results = t->childIntersections(r);

    EXPECT_EQ(results.size(), 0);
}

TEST(Triangle_ChildIntersectionTest, RayMissesP1P3Edge){
    Triangle* t = new Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(1, 1, -2), Vector(0, 0, 1));

    std::vector<Intersection> results = t->childIntersections(r);

    EXPECT_EQ(results.size(), 0);
}

TEST(Triangle_ChildIntersectionTest, RayMissesP1P2Edge){
    Triangle* t = new Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(-1, 1, -2), Vector(0, 0, 1));

    std::vector<Intersection> results = t->childIntersections(r);

    EXPECT_EQ(results.size(), 0);
}

TEST(Triangle_ChildIntersectionTest, RayMissesP2P3Edge){
    Triangle* t = new Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(0, -1, -2), Vector(0, 0, 1));

    std::vector<Intersection> results = t->childIntersections(r);

    EXPECT_EQ(results.size(), 0);
}

TEST(Triangle_ChildIntersectionTest, RayIntersectsTriangle){
    Triangle* t = new Triangle(Point(0, 1, 0), Point(-1, 0, 0), Point(1, 0, 0));
    Ray r(Point(0, 0.5, -2), Vector(0, 0, 1));

    std::vector<Intersection> results = t->childIntersections(r);

    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.at(0).getTime(), 2);
}