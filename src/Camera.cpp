#include "Camera.h"

Camera::Camera(int w, int h, float fov){
    width = w;
    height = h;
    this->fov = fov;
    transform = Matrix(4);
}

int Camera::getWidth(){
    return width;
}

int Camera::getHeight(){
    return height;
}

float Camera::getFOV(){
    return fov;
}

Matrix Camera::getTransform(){
    return transform;
}