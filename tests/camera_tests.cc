#include <gtest/gtest.h>
#include "Camera.h"
#include "common.h"

TEST(CameraTest, BasicTest){
    Camera c(160, 120, PI/2);
    EXPECT_EQ(c.getHSize(), 160);
    EXPECT_EQ(c.getVSize(), 120);
    EXPECT_EQ(c.getFOV(), PI/2);
    EXPECT_TRUE(c.getTransform().isEqual(Matrix(4)));

    c = Camera(200, 125, PI/2);
    EXPECT_TRUE(floatIsEqual(c.getPixelSize(), 0.01));
    c = Camera(125, 200, PI/2);
    EXPECT_TRUE(floatIsEqual(c.getPixelSize(), 0.01));
}

TEST(CameraTest, RayToPixelTest){
    Camera c(201, 101, PI/2);
    Ray r = c.rayToPixel(100, 50);
    EXPECT_TRUE(r.getOrigin().isEqual(Point()));
    EXPECT_TRUE(r.getDirection().isEqual(Vector(0, 0, -1)));

    r = c.rayToPixel(0, 0);
    EXPECT_TRUE(r.getOrigin().isEqual(Point()));
    EXPECT_TRUE(r.getDirection().isEqual(Vector(0.66519, 0.33259, -0.66851)));

    c.setTransform(yRotationMatrix(PI/4)*translationMatrix(0, -2, 5));
    r = c.rayToPixel(100, 50);
    EXPECT_TRUE(r.getOrigin().isEqual(Point(0, 2, -5)));
    EXPECT_TRUE(r.getDirection().isEqual(Vector(sqrt(2)/2, 0, -sqrt(2)/2)));
}

TEST(CameraTest, RenderTest){
    World w = defaultWorld();
    Camera c(11, 11, PI/2);
    Point cameraPosition(0, 0, -5);
    Point to;
    Vector up(0, 1, 0);
    c.setTransform(viewTransformationMatrix(cameraPosition, to, up));

    Canvas image = c.render(w);
    Colour a = image.pixelColour(5, 5);
    EXPECT_TRUE(a.isEqual(Colour(0.38066, 0.47583, 0.2855)));
}