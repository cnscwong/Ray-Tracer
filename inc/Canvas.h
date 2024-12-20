#pragma once
#include "Colour.h"
#include "common.h"
#include <string>

const int DEFAULT_WIDTH = 100;
const int DEFAULT_HEIGHT = 100;

// Class to render and modify generated images
class Canvas{
private:
    int width, height;
    Colour **pixels;
public:
    Canvas();
    Canvas(int w, int h);
    ~Canvas();

    int getWidth();
    int getHeight();
    Colour pixelColour(int x, int y);
    void write_pixel(int x, int y, Colour c);
    void setAllPixels(Colour c);
    std::string toPPM();
};