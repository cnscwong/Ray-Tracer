#pragma once
#include "Colour.h"
#include "common.h"
#include <string>
#include <iostream>
#include <fstream>

const int DEFAULT_WIDTH = 100;
const int DEFAULT_HEIGHT = 100;

// Class to render and modify generated images
class Canvas{
private:
    // Width and height of canvas
    int width, height;
    // 2D array storing the colour properties of each pixel
    Colour **pixels;
public:
    // Canvas constructors and destructor
    Canvas();
    Canvas(int w, int h);
    ~Canvas();

    // Getters and setters
    int getWidth();
    int getHeight();
    Colour pixelColour(int x, int y);
    void write_pixel(int x, int y, Colour c);
    void setAllPixels(Colour c);

    // Produces the canvas as a ppm file string
    std::string toPPM();
    void writeToFile(std::string file_name);
};