#include <iostream>
#include "Tuple.h"
#include "Shape.h"
#include "Canvas.h"
#include "Colour.h"
#include "Ray.h"
#include "LightAndShading.h"
#include "World.h"
#include "Camera.h"
#include "common.h"
#include "Pattern.h"
#include <cmath>

int main(){
    CheckerPattern* p1 = new CheckerPattern({WHITE, Colour(0.1, 0.3, 0.9)});
    LinearGradient* p2 = new LinearGradient({Colour(0.34, 0.89, 0.89), Colour(0.31, 1, 0.44)});
    p2->setTransform(translationMatrix(1.5, -1, -0.5)*scalingMatrix(2.5, 1, 1));
    RingPattern* p3 = new RingPattern({Colour(0, 0, 1), Colour(0.5, 0, 1)});
    p3->setTransform(translationMatrix(-1.5, -0.5, 0.5));
    Stripes* p4 = new Stripes({Colour(1, 0.65, 0), Colour(1, 1, 0)});

    Plane* floor = new Plane;
    Material m;
    m.colour = Colour(1, 0.9, 0.9);
    m.pattern = p1;
    floor->setMaterial(m);

    Cone* middle = new Cone;
    middle->setClosed(true);
    middle->setMinH(-3);
    middle->setTransform(translationMatrix(-0.5, 1, 0.5));
    m = Material();
    m.colour = Colour(0.1, 1, 0.5);
    m.diffuse = 0.7;
    m.specular = 0.3;
    m.pattern = p2;
    middle->setMaterial(m);

    Sphere* right = new Sphere;
    right->setTransform(translationMatrix(1.5, 0.5, -0.5)*scalingMatrix(0.5, 0.5, 0.5));
    m.colour = Colour(0.5, 1, 0.1);
    m.pattern = p3;
    right->setMaterial(m);

    Sphere* left = new Sphere;
    left->setTransform(translationMatrix(-1.5, 0.33, -0.75)*scalingMatrix(0.33, 0.33, 0.33));
    m.colour = Colour(1, 0.8, 0.1);
    m.pattern = p4;
    left->setMaterial(m);

    World w;
    w.setLight(LightSource(Point(-10, 10, -10), Colour(1, 1, 1)));
    // w.appendObject(floor);
    w.appendObject(middle);
    w.appendObject(right);
    w.appendObject(left);

    Camera c(100, 50, PI/3);
    c.setTransform(viewTransformationMatrix(Point(0, -6, -10), Point(0, 1, 0), Vector(0, 1, 0)));
    Canvas canvas = c.render(w);

    canvas.writeToFile("out.ppm");

    delete floor;
    delete middle;
    delete right;
    delete left;
    floor = nullptr;
    middle = nullptr;
    right = nullptr;
    left = nullptr;

    return 0;
}