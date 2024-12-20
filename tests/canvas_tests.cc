#include <gtest/gtest.h>
#include "Canvas.h"

TEST(CanvasTests, BasicTest){
    Canvas a(10, 20);

    int w = a.getWidth();
    int h = a.getHeight();
    EXPECT_EQ(w, 10);
    EXPECT_EQ(h, 20);

    for(int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
            EXPECT_TRUE(a.pixelColour(x, y).isEqual(Colour(0, 0, 0)));
        }
    }
}

TEST(CanvasTests, WritePixel){
    Canvas a(10, 20);
    Colour red(1, 0, 0);
    a.write_pixel(2, 3, red);

    EXPECT_TRUE(a.pixelColour(2, 3).isEqual(Colour(1, 0, 0)));
}

TEST(CanvasTests, SetAllPixels){
    Canvas a(10, 20);
    Colour red(1, 0, 0);
    a.setAllPixels(red);

    for(int x = 0; x < a.getWidth(); x++){
        for(int y = 0; y < a.getHeight(); y++){
            EXPECT_TRUE(a.pixelColour(x, y).isEqual(Colour(1, 0, 0)));
        }
    }
}

TEST(PPMTests, BasicOutput){
    Canvas a(5, 3);

    a.write_pixel(0, 0, Colour(1.5, 0, 0));
    a.write_pixel(2, 1, Colour(0, 0.5, 0));
    a.write_pixel(4, 2, Colour(-0.5, 0, 1));

    EXPECT_EQ(a.toPPM(), "P3\n5 3\n255\n255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n");
}

// Testing lines that exceed 70 characters are split into multiple lines
TEST(PPMTests, LineCharacterLimit){
    Canvas a(10, 2);
    a.setAllPixels(Colour(1, 0.8, 0.6));

    EXPECT_EQ(a.toPPM(), "P3\n10 2\n255\n255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n153 255 204 153 255 204 153 255 204 153 255 204 153\n255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n153 255 204 153 255 204 153 255 204 153 255 204 153\n");
}