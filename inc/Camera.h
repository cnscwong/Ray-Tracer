#pragma once
#include "Matrix.h"
#include "Ray.h"
#include "Canvas.h"
#include "World.h"
#include <stdexcept>

// Class representing a virtual camera that you are able to move around the world.
// Actually, moving the world relative to the camera by multiplying the inverse of
// the tranform variable with the world. The camera is at the origin "looking" at a
// canvas one unit away in the -z direction
class Camera{
private:
    // horizontal and vertical size of the canvas in pixels
    int hsize;
    int vsize;
    // half the horizontal and vertical size of the canvas in world units
    float half_width;
    float half_height;
    // side length of a pixel(pixel is a square) in world units
    float pixel_size;
    // Angle that the camera can see, smaller will result in the view being more zoomed in
    float fov;
    // transform representing how much we have to orient the world
    // if we wanted to compute the position of the camera, we would
    // multiply inverse(transform)*Point(0, 0, 0) assuming default camera
    // starts at origin
    Matrix transform;
public:
    // Camera constructor
    Camera(int h, int v, float fov);

    // Camera getters
    int getHSize();
    int getVSize();
    float getFOV();
    Matrix getTransform();
    float getPixelSize();

    // Camera setters
    void setTransform(Matrix m);

    // Computes pixel size in world units
    void computePixelSize();

    // Computes a ray that starts at the camera and goes through the canvas at the specified xy pixel
    Ray rayToPixel(int x, int y);

    // Produces the rendered canvas for the given world based off of the camera and world properties
    Canvas render(World w);
};