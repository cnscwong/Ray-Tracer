#include <gtest/gtest.h>
#include "Colour.h"
#include "common.h"

TEST(ColourTests, BasicTest){
    Colour a(-0.5, 0.4, 1.7);

    EXPECT_NEAR(a.r, -0.5, EPSILON);
    EXPECT_NEAR(a.g, 0.4, EPSILON);
    EXPECT_NEAR(a.b, 1.7, EPSILON);
}

TEST(ColourTests, ColourOperations){
    Colour a(0.9, 0.6, 0.75);
    Colour b(0.7, 0.1, 0.25);

    EXPECT_TRUE(addColours(a, b).isEqual(Colour(1.6, 0.7, 1.0)));
    EXPECT_TRUE(subtractColours(a, b).isEqual(Colour(0.2, 0.5, 0.5)));

    Colour c(0.2, 0.3, 0.4);
    EXPECT_TRUE(scaleColour(c, 2).isEqual(Colour(0.4, 0.6, 0.8)));

    a = Colour(1, 0.2, 0.4);
    b = Colour(0.9, 1, 0.1);
    EXPECT_TRUE(multiplyColour(a, b).isEqual(Colour(0.9, 0.2, 0.04)));
}