#include <gtest/gtest.h>
#include "World.h"
#include "Ray.h"
#include "Shape.h"

TEST(WorldTest, BasicTest){
    World w;
    LightSource l(Point(0, 0, 0), Colour(0, 0, 0));
    EXPECT_EQ(w.getObjects().size(), 0);
    EXPECT_TRUE(w.getLight().isEqual(l));

    w = defaultWorld();
    l = LightSource(Point(-10, 10, -10), Colour(1, 1, 1));

    Sphere* s1 = new Sphere;
    Material m;
    m.colour = Colour(0.8, 1.0, 0.6);
    m.diffuse = 0.7;
    m.specular = 0.2;
    s1->setMaterial(m);

    Sphere* s2 = new Sphere;
    s2->setTransform(scalingMatrix(0.5, 0.5, 0.5));
    EXPECT_EQ(w.getObjects().size(), 2);
    EXPECT_TRUE(w.getObjects().at(0)->isEqual(s1));
    EXPECT_TRUE(w.getObjects().at(1)->isEqual(s2));
    EXPECT_TRUE(w.getLight().isEqual(l));
    delete s1;
    delete s2;
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
    Sphere* s = dynamic_cast<Sphere*>(w.getObjects().at(0));
    Intersection i(4, s);
    LightData data = prepareLightData(i, r);
    Colour c = w.shadeHit(data);
    EXPECT_TRUE(c.isEqual(Colour(0.38066, 0.47583, 0.2855)));

    w = defaultWorld();
    w.setLight(LightSource(Point(0, 0.25, 0), Colour(1, 1, 1)));
    r = Ray(Point(), Vector(0, 0, 1));
    s = dynamic_cast<Sphere*>(w.getObjects().at(1));
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
    // EXPECT_TRUE(c.isEqual(Colour(0.38066, 0.47583, 0.2855)));

    // std::vector<Shape*> objs = w.getObjects();
    // Material m = objs.at(0)->getMaterial();
    // m.setAmbient(1);
    // objs.at(0)->setMaterial(m);
    // m = objs.at(0)->getMaterial();
    // m.setAmbient(1);
    // objs.at(1)->setMaterial(m);
    // w.setObjects(objs);
    // r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
    // c = w.colourAtHit(r);
    // EXPECT_TRUE(c.isEqual(w.getObjects().at(1)->getMaterial().colour));
}

TEST(WorldTest, ViewTransformationTest){
    // The world does not require any scaling, rotating, or translation so the result is the identity matrix
    Point cameraPosition(0, 0, 0);
    Point to(0, 0, -1);
    Vector up(0, 1, 0);
    EXPECT_TRUE(viewTransformationMatrix(cameraPosition, to, up).isEqual(Matrix(4)));

    // Since reflection is the same as scaling by negative values, turning the camera to look at the 
    // positive z direction will result in the scalingMatrix(-1, 1, -1)
    to = Point(0, 0, 1);
    EXPECT_TRUE(viewTransformationMatrix(cameraPosition, to, up).isEqual(scalingMatrix(-1, 1, -1)));

    // The resulting matrix moves the world back 8 units
    cameraPosition = Point(0, 0, 8);
    to = Point();
    EXPECT_TRUE(viewTransformationMatrix(cameraPosition, to, up).isEqual(translationMatrix(0, 0, -8)));

    // Matrix that is a combination of shearing, scaling, and translation
    cameraPosition = Point(1, 3, 2);
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
    EXPECT_TRUE(viewTransformationMatrix(cameraPosition, to, up).isEqual(m));
}

TEST(WorldTest, ShadowTest){
    Material m;
    Point position;
    Vector camera = Vector(0, 0, -1);
    Vector normal = Vector(0, 0, -1);
    LightSource light = LightSource(Point(0, 0, -10), Colour(1, 1, 1));

    Shape* s = new Shape;

    Colour c = computeLighting(m, s, light, position, camera, normal, true);
    EXPECT_TRUE(c.isEqual(Colour(0.1, 0.1, 0.1)));
    delete s;
    s = nullptr;

    // Nothing between point and light
    World w = defaultWorld();
    Point p(0, 10, 0);
    EXPECT_FALSE(w.hasShadow(p));

    // Object between the point and light
    p = Point(10, -10, 10);
    EXPECT_TRUE(w.hasShadow(p));

    // Object behind line towards light source
    p = Point(-20, 20, -20);
    EXPECT_FALSE(w.hasShadow(p));

    // Object behind the point
    p = Point(-2, 2, -2);
    EXPECT_FALSE(w.hasShadow(p));

    w = World();
    Sphere* s1 = new Sphere;
    Sphere* s2 = new Sphere;
    w.setLight(LightSource(Point(0, 0, -10), Colour(1, 1, 1)));
    w.appendObject(s1);
    s2->setTransform(translationMatrix(0, 0, 10));
    w.appendObject(s2);

    Ray r(Point(0, 0, 5), Vector(0, 0, 1));
    Intersection i(4, s2);
    c = w.shadeHit(prepareLightData(i, r));
    EXPECT_TRUE(c.isEqual(Colour(0.1, 0.1, 0.1)));
    delete s1;
    delete s2;
}

TEST(WorldTest, reflectedColourTestNonreflectiveSurface){
    World w = defaultWorld();
    Ray r(Point(), Vector(0, 0, 1));
    std::vector<Shape*> objects = w.getObjects();
    Material m = objects.at(1)->getMaterial();
    m.ambient = 1;
    objects.at(1)->setMaterial(m);
    Intersection i(1, objects.at(1));
    LightData data = prepareLightData(i, r);
    Colour c = w.reflectedColour(data);
    EXPECT_TRUE(c.isEqual(BLACK));
}

TEST(WorldTest, reflectedColourTestReflectiveSurface){
    World w = defaultWorld();
    Plane* p = new Plane;
    Material m;
    m.reflective = 0.5;
    p->setMaterial(m);
    p->setTransform(translationMatrix(0, -1, 0));
    w.appendObject(p);

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2)/2, sqrt(2)/2));
    Intersection i(sqrt(2), p);
    LightData data = prepareLightData(i, r);
    Colour c = w.reflectedColour(data);
    
    EXPECT_TRUE(c.isEqual(Colour(0.19032, 0.2379, 0.14274)));
}

TEST(WorldTest, reflectedColourInShadeHit){
    World w = defaultWorld();
    Plane* p = new Plane;
    Material m;
    m.reflective = 0.5;
    p->setMaterial(m);
    p->setTransform(translationMatrix(0, -1, 0));
    w.appendObject(p);

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2)/2, sqrt(2)/2));
    Intersection i(sqrt(2), p);

    LightData data = prepareLightData(i, r);
    Colour c = w.shadeHit(data);
    EXPECT_TRUE(c.isEqual(Colour(0.87677, 0.92436, 0.82918)));
}

TEST(WorldTest, ColourAtAvoidsInfiniteRecursion){
    World w;
    w.setLight(LightSource(Point(), WHITE));
    Plane* l = new Plane;
    Plane* u = new Plane;
    Material m;
    m.reflective = 1;
    l->setMaterial(m);
    u->setMaterial(m);
    l->setTransform(translationMatrix(0, -1, 0));
    u->setTransform(translationMatrix(0, 1, 0));
    w.appendObject(l);
    w.appendObject(u);

    Ray r(Point(), Vector(0, 1, 0));
    w.colourAtHit(r);
}

TEST(WorldTest, ReflectedColourReflectsBlackAtRecursiveLimit){
    World w = defaultWorld();
    Plane* p = new Plane;
    Material m;
    m.reflective = 0.5;
    p->setMaterial(m);
    p->setTransform(translationMatrix(0, -1, 0));
    w.appendObject(p);

    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2)/2, sqrt(2)/2));
    Intersection i(sqrt(2), p);
    LightData data = prepareLightData(i, r);
    Colour c = w.reflectedColour(data, 0);

    EXPECT_TRUE(c.isEqual(BLACK));
}

TEST(WorldTest, RefractedColourIsBlackOnOpaqueSurace){
    World w = defaultWorld();
    Shape* shape = w.getObjects().at(0);
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    std::vector<Intersection> intersects({Intersection(4, shape), Intersection(6, shape)});
    LightData data = prepareLightData(intersects.at(0), r, intersects);
    Colour c = w.refractedColour(data);

    EXPECT_TRUE(c.isEqual(BLACK));
}

TEST(WorldTest, RefractedColourIsBlackAtRecursiveLimit){
    World w = defaultWorld();
    Shape* shape = w.getObjects().at(0);
    Material m;
    m.transparency = 1;
    shape->setMaterial(m);
    Ray r(Point(0, 0, -5), Vector(0, 0, 1));
    std::vector<Intersection> intersects({Intersection(4, shape), Intersection(6, shape)});
    LightData data = prepareLightData(intersects.at(0), r, intersects);
    Colour c = w.refractedColour(data, 0);

    EXPECT_TRUE(c.isEqual(BLACK));
}

TEST(WorldTest, TotalInternalReflectionOccurs_RefractedColourReturnsBlack){
    World w = defaultWorld();
    Shape* shape = w.getObjects().at(0);
    Material m;
    m.transparency = 1;
    m.refractiveIndex = 1.5;
    shape->setMaterial(m);
    Ray r(Point(0, 0, sqrt(2)/2), Vector(0, 1, 0));
    std::vector<Intersection> intersects({Intersection(-sqrt(2)/2, shape), Intersection(sqrt(2)/2, shape)});
    LightData data = prepareLightData(intersects.at(1), r, intersects);
    Colour c = w.refractedColour(data);

    EXPECT_TRUE(c.isEqual(BLACK));
}

TEST(WorldTest, RefractedColourCorrectForRefractedRay){
    World w = defaultWorld();

    Pattern* p = new Pattern;
    Material m;
    m.ambient = 1.0;
    m.pattern = p;
    Shape* A = w.getObjects().at(0);
    A->setMaterial(m);

    m = Material();
    m.transparency = 1.0;
    m.refractiveIndex = 1.5;
    Shape* B = w.getObjects().at(1);
    B->setMaterial(m);

    Ray r(Point(0, 0, 0.1), Vector(0, 1, 0));
    std::vector<Intersection> intersects({Intersection(-0.9899, A), Intersection(-0.4899, B), Intersection(0.4899, B), Intersection(0.9899, A)});
    LightData data = prepareLightData(intersects.at(2), r, intersects);
    Colour c = w.refractedColour(data);

    // Using the default pattern, we can check what point the refracted ray hit here
    EXPECT_TRUE(c.isEqual(Colour(0, 0.99888, 0.04725)));
}

TEST(WorldTest, ShadeHitHandlesRefraction){
    World w = defaultWorld();

    Material m;
    m.transparency = 0.5;
    m.refractiveIndex = 1.5;
    Plane* floor = new Plane;
    floor->setTransform(translationMatrix(0, -1, 0));
    floor->setMaterial(m);

    m = Material();
    m.colour = Colour(1, 0, 0);
    m.ambient = 0.5;
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(0, -3.5, -0.5));
    s->setMaterial(m);

    w.appendObject(floor);
    w.appendObject(s);
    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2)/2, sqrt(2)/2));
    std::vector<Intersection> intersects({Intersection(sqrt(2), floor)});
    LightData data = prepareLightData(intersects.at(0), r, intersects);

    Colour c = w.shadeHit(data, 5);
    EXPECT_TRUE(c.isEqual(Colour(0.93642, 0.68642, 0.68642)));
}

TEST(WorldTest, ShadeHitUsesReflectanceWhenCombiningReflectionAndRefraction){
    World w = defaultWorld();

    Material m;
    m.reflective = 0.5;
    m.transparency = 0.5;
    m.refractiveIndex = 1.5;
    Plane* floor = new Plane;
    floor->setTransform(translationMatrix(0, -1, 0));
    floor->setMaterial(m);

    m = Material();
    m.colour = Colour(1, 0, 0);
    m.ambient = 0.5;
    Sphere* s = new Sphere;
    s->setTransform(translationMatrix(0, -3.5, -0.5));
    s->setMaterial(m);

    w.appendObject(floor);
    w.appendObject(s);
    Ray r(Point(0, 0, -3), Vector(0, -sqrt(2)/2, sqrt(2)/2));
    std::vector<Intersection> intersects({Intersection(sqrt(2), floor)});
    LightData data = prepareLightData(intersects.at(0), r, intersects);

    Colour c = w.shadeHit(data, 5);
    EXPECT_TRUE(c.isEqual(Colour(0.93391, 0.69643, 0.69243)));
}