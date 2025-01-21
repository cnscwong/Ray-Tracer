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
    Stripes* stripes = new Stripes({WHITE, Colour(1, 0, 0)});
    stripes->transform = zRotationMatrix(PI/4)*xRotationMatrix(PI/2);

    // Floor is actually just an extremely flattened sphere
    Plane* floor = new Plane;
    Material m;
    m.setColour(Colour(1, 0.9, 0.9));
    m.setPattern(stripes);
    floor->setMaterial(m);

    Sphere* middle = new Sphere;
    middle->setTransform(translationMatrix(-0.5, 1, 0.5)*zRotationMatrix(PI/2));
    m = Material();
    m.setColour(Colour(0.1, 1, 0.5));
    m.setDiffuse(0.7);
    m.setSpecular(0.3);
    m.setPattern(stripes);
    middle->setMaterial(m);

    Sphere* right = new Sphere;
    right->setTransform(translationMatrix(1.5, 0.5, -0.5)*scalingMatrix(0.5, 0.5, 0.5));
    m.setColour(Colour(0.5, 1, 0.1));
    right->setMaterial(m);

    Sphere* left = new Sphere;
    left->setTransform(translationMatrix(-1.5, 0.33, -0.75)*scalingMatrix(0.33, 0.33, 0.33));
    m.setColour(Colour(1, 0.8, 0.1));
    left->setMaterial(m);

    World w;
    w.setLight(LightSource(Point(-10, 10, -10), Colour(1, 1, 1)));
    w.appendObject(floor);
    w.appendObject(middle);
    w.appendObject(right);
    w.appendObject(left);

    Camera c(100, 50, PI/3);
    c.setTransform(viewTransformationMatrix(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0)));
    Canvas canvas = c.render(w);

    canvas.writeToFile("out.ppm");

    delete stripes;
    delete floor;
    delete middle;
    delete right;
    delete left;
    stripes = nullptr;
    floor = nullptr;
    middle = nullptr;
    right = nullptr;
    left = nullptr;

    return 0;
}