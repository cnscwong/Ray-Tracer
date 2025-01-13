#include "Camera.h"

// Camera constructor
Camera::Camera(int h, int v, float fov){
    hsize = h;
    vsize = v;
    this->fov = fov;
    transform = Matrix(4);
    computePixelSize();
}

// Getter variables for camera
int Camera::getHSize(){
    return hsize;
}

int Camera::getVSize(){
    return vsize;
}

float Camera::getFOV(){
    return fov;
}

Matrix Camera::getTransform(){
    return transform;
}

float Camera::getPixelSize(){
    return pixel_size;
}

// Setter variables for camera
void Camera::setTransform(Matrix m){
    transform = m;
}

// Computes the size of a pixel in the units of the world eg. if the pixel size is 0.01 then 
// a pixel is 0.01 unit x 0.01 unit square in the world. Intuitively, you can think of the canvas
// as being a window in front of the camera and the window dimensions is width*p_size x height*p_size.
// The canvas is one unit in front of the camera and the camera has to shoot rays through this window
// where each ray goes through the canvas and each ray is associated with one pixel. The computed 
// colour the ray returns from hitting objects is the colour of the pixel
void Camera::computePixelSize(){
    // I am using FOV as the angle that can be seen by the longer dimension of the canvas
    // half_view variable refers to the number of world units that the longer side of the 
    // camera can see
    float half_view = tan(fov/2);
    // Calculates aspect ratio which is ratio of the horizontal size to the vertical size of the canvas
    float aspect_ratio = 1.0*hsize/vsize;

    // Width is longer so half_view corresponds to width, else half_view corresponds to half_height
    if(aspect_ratio >= 1){
        half_width = half_view;
        half_height = half_view/aspect_ratio;
    }else{
        half_width = half_view*aspect_ratio;
        half_height = half_view;
    }

    pixel_size = half_width*2/hsize;
}

// Computes a ray that starts at the camera and goes through the canvas at the specified xy pixel
Ray Camera::rayToPixel(int x, int y){
    if(x < 0 || x >= hsize || y < 0 || y >= vsize){
        throw std::invalid_argument("rayToPixel xy invalid");
    }

    // Offset of canvas edge to center of pixel
    float xOffset = (x + 0.5)*pixel_size;
    float yOffset = (y + 0.5)*pixel_size;

    // half_width is the largest x value for rays that go through the canvas,
    // half_height is the largest y value for rays that go through the canvas.
    // Subtracting by the offset will represent the xy coordinate from the corner
    // (max(x), max(y)) of the canvas
    float xWorld = half_width - xOffset;
    float yWorld = half_height - yOffset;

    // transforms the canvas point and camera origin to their
    // world positions
    Point pixel = Point(transform.inverse()*Point(xWorld, yWorld, -1));
    Point origin = Point(transform.inverse()*Point());
    Vector direction = Vector(pixel - origin).normalize();

    return Ray(origin, direction);
}

// Renders the world using the camera and world properties
Canvas Camera::render(World w){
    Canvas image(hsize, vsize);
    Ray r;
    Colour col;

    for(int y = 0; y < vsize; y++){
        for(int x = 0; x < hsize; x++){
            r = this->rayToPixel(x, y);
            col = w.colourAtHit(r);
            image.write_pixel(x, y, col);
        }
    }

    return image;
}