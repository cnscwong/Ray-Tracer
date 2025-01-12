#pragma once
#include "Matrix.h"

// Class representing a virtual camera that you are able to move around the "world"
class Camera
{
private:
    // width and height of the camera's pixels
    int width;
    int height;
    // Angle that the camera can see, smaller will result in the view being more zoomed in
    float fov;
    // transform representing how much we have to orient the world
    // will be the result of viewTransformationMatrix function
    Matrix transform;
public:
    // Camera constructor
    Camera(int w, int h, float fov);

    // Camera getters
    int getWidth();
    int getHeight();
    float getFOV();
    Matrix getTransform();
};