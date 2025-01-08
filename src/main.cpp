#include <iostream>
#include "Tuple.h"
#include "Sphere.h"
#include "Canvas.h"
#include "Colour.h"
#include "Ray.h"
#include "LightAndShading.h"

int main(){
    Point ray_origin(0, 0, -5);
    Sphere s;
    Material m;
    m.setColour(Colour(1, 0.2, 1));
    s.setMaterial(m);

    LightSource light(Point(-10, 10, -10), Colour(1, 1, 1));
    float wall_z = 10;
    float wall_size = 7;

    int canvas_pixels = 1000;
    Canvas c(canvas_pixels, canvas_pixels);
    float pixel_size = wall_size/canvas_pixels;
    float half = wall_size/2;

    Colour col(1, 0, 0);
    float y_coord, x_coord;
    Point position;
    Ray r;
    Vector v;
    std::vector<Intersection> intersects;
    int hitInd;
    Vector normalV, eyeV;

    for(int y = 0; y < canvas_pixels; y++){
        y_coord = half - pixel_size*y;

        for(int x = 0; x < canvas_pixels; x++){
            x_coord = -half + pixel_size*x;
            position = Point(x_coord, y_coord, wall_z);
            v = Vector(position - ray_origin);
            r = Ray(ray_origin, v.normalize());

            intersects = r.RaySphereIntersection(s);
            hitInd = hit(intersects);
            if(hitInd != -1){
                position = r.computePosition(intersects.at(hitInd).getTime());
                normalV = intersects.at(hitInd).getSphere().computeNormal(position);
                eyeV = Vector(r.getDirection().negateTuple());
                col = computeLighting(intersects.at(hitInd).getSphere().getMaterial(), position, light, eyeV, normalV);
                c.write_pixel(x, y, col);
            }
        }
    }

    c.writeToFile("out.ppm");

    return 0;
}