#include <iostream>
#include "Tuple.h"
#include "Sphere.h"
#include "Canvas.h"
#include "Colour.h"
#include "Ray.h"
#include "LightAndShading.h"
#include "World.h"
#include "Camera.h"
#include "common.h"
#include <cmath>

int main(){
    // Floor is actually just an extremely flattened sphere
    Sphere floor;
    floor.setTransform(scalingMatrix(10, 0.01, 10));
    Material m;
    m.setColour(Colour(1, 0.9, 0.9));
    floor.setMaterial(m);

    Sphere left_wall;
    left_wall.setTransform(translationMatrix(0, 0, 5)*yRotationMatrix(-PI/4)*xRotationMatrix(PI/2)*scalingMatrix(10, 0.01, 10));
    left_wall.setMaterial(m);

    Sphere right_wall;
    right_wall.setTransform(translationMatrix(0, 0, 5)*yRotationMatrix(PI/4)*xRotationMatrix(PI/2)*scalingMatrix(10, 0.01, 10));
    right_wall.setMaterial(m);

    Sphere middle;
    middle.setTransform(translationMatrix(-0.5, 1, 0.5));
    m = Material();
    m.setColour(Colour(0.1, 1, 0.5));
    m.setDiffuse(0.7);
    m.setSpecular(0.3);
    middle.setMaterial(m);

    Sphere right;
    right.setTransform(translationMatrix(1.5, 0.5, -0.5)*scalingMatrix(0.5, 0.5, 0.5));
    m.setColour(Colour(0.5, 1, 0.1));
    right.setMaterial(m);

    Sphere left;
    left.setTransform(translationMatrix(-1.5, 0.33, -0.75)*scalingMatrix(0.33, 0.33, 0.33));
    m.setColour(Colour(1, 0.8, 0.1));
    left.setMaterial(m);

    World w;
    w.setLight(LightSource(Point(-10, 10, -10), Colour(1, 1, 1)));
    w.appendObject(floor);
    w.appendObject(left_wall);
    w.appendObject(right_wall);
    w.appendObject(middle);
    w.appendObject(right);
    w.appendObject(left);

    Camera c(100, 50, PI/3);
    c.setTransform(viewTransformationMatrix(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0)));
    Canvas canvas = c.render(w);

    canvas.writeToFile("out.ppm");

    return 0;
}