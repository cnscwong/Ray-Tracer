#include <gtest/gtest.h>
#include "Pattern.h"
#include "Matrix.h"
#include "common.h"
#include "LightAndShading.h"
#include "Shape.h"

TEST(PatternTest, PatternConstructor){
    // Arrange
    Pattern p;

    // Act
    bool defaultPattern = p.getTransform().isEqual(Matrix(4));

    // Assert
    EXPECT_TRUE(defaultPattern);
}

TEST(PatternTest, SetTransform_SetCorrectly){
    // Arrange
    Pattern p;

    // Act
    p.setTransform(translationMatrix(1, 2, 3));

    // Assert
    EXPECT_TRUE(p.getTransform().isEqual(translationMatrix(1, 2, 3)));
}

TEST(PatternTest, ApplyPattern_ObjectTransformationAppliedCorrectly){
    // Arrange
    Sphere* s = new Sphere;
    s->setTransform(scalingMatrix(2, 2, 2));
    Pattern p;

    // Act
    Colour c = p.applyPattern(s, Point(2, 3, 4));

    // Assert
    EXPECT_TRUE(c.isEqual(Colour(1, 1.5, 2)));
}

TEST(PatternTest, ApplyPattern_PatternTransformationAppliedCorrectly){
    // Arrange
    Sphere* s = new Sphere;
    Pattern p;
    p.setTransform(scalingMatrix(2, 2, 2));

    // Act
    Colour c = p.applyPattern(s, Point(2, 3, 4));

    // Assert
    EXPECT_TRUE(c.isEqual(Colour(1, 1.5, 2)));
}

TEST(PatternTest, ApplyPattern_ObjectAndPatternTransformationAppliedCorrectly){
    // Arrange
    Sphere* s = new Sphere;
    s->setTransform(scalingMatrix(2, 2, 2));
    Pattern p;
    p.setTransform(translationMatrix(0.5, 1, 1.5));

    // Act
    Colour c = p.applyPattern(s, Point(2.5, 3, 3.5));

    // Assert
    EXPECT_TRUE(c.isEqual(Colour(0.75, 0.5, 0.25)));
}

TEST(StripesTest, StripesConstructor){
    // Arrange
    Stripes s1;
    Stripes s2 = Stripes({Colour(1, 0, 0), Colour(0, 1, 0), Colour(0, 0, 1)});
    
    // Act
    bool s1TransformIsIdentityMatrix = s1.getTransform().isEqual(Matrix(4));
    bool s1HasTwoStripes = s1.colours.size() == 2;
    bool firstS1StripeIsWhite = s1.colours.at(0).isEqual(WHITE);
    bool secondS1StripeIsBlack = s1.colours.at(1).isEqual(BLACK);

    bool s2TransformIsIdentityMatrix = s2.getTransform().isEqual(Matrix(4));
    bool s2HasThreeStripes = s2.colours.size() == 3;
    bool firstS2StripeIsRed = s2.colours.at(0).isEqual(Colour(1, 0, 0));
    bool secondS2StripeIsGreen = s2.colours.at(1).isEqual(Colour(0, 1, 0));
    bool thirdS2StripeIsBlue = s2.colours.at(2).isEqual(Colour(0, 0, 1));

    // Assert
    EXPECT_TRUE(s1TransformIsIdentityMatrix);
    EXPECT_TRUE(s1HasTwoStripes);
    EXPECT_TRUE(firstS1StripeIsWhite);
    EXPECT_TRUE(secondS1StripeIsBlack);

    EXPECT_TRUE(s2TransformIsIdentityMatrix);
    EXPECT_TRUE(s2HasThreeStripes);
    EXPECT_TRUE(firstS2StripeIsRed);
    EXPECT_TRUE(secondS2StripeIsGreen);
    EXPECT_TRUE(thirdS2StripeIsBlue);
}

TEST(StripesTest, StripesChildApplyPattern){
    Stripes s;

    EXPECT_TRUE(s.ChildApplyPattern(Point()).isEqual(WHITE));

    // y changes
    EXPECT_TRUE(s.ChildApplyPattern(Point(0, 1, 0)).isEqual(WHITE));
    EXPECT_TRUE(s.ChildApplyPattern(Point(0, 2, 0)).isEqual(WHITE));

    // z changes
    EXPECT_TRUE(s.ChildApplyPattern(Point(0, 0, 1)).isEqual(WHITE));
    EXPECT_TRUE(s.ChildApplyPattern(Point(0, 0, 2)).isEqual(WHITE));

    // x changes
    EXPECT_TRUE(s.ChildApplyPattern(Point(0.9, 0, 0)).isEqual(WHITE));
    EXPECT_TRUE(s.ChildApplyPattern(Point(1, 0, 0)).isEqual(BLACK));
    EXPECT_TRUE(s.ChildApplyPattern(Point(-0.1, 0, 0)).isEqual(BLACK));
    EXPECT_TRUE(s.ChildApplyPattern(Point(-1, 0, 0)).isEqual(BLACK));
    EXPECT_TRUE(s.ChildApplyPattern(Point(-1.1, 0, 0)).isEqual(WHITE));
}

TEST(FunctionTest, ComputeLighting_AppliesStripePattern){
    Stripes* stripes = new Stripes;
    Sphere* s = new Sphere;
    Material m;
    m.pattern = stripes;
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;

    Vector camera(0, 0, -1);
    Vector normal(0, 0, -1);

    LightSource light(Point(0, 0, -10), WHITE);
    Colour c1 = computeLighting(m, s, light, Point(0.9, 0, 0), camera, normal, false);
    Colour c2 = computeLighting(m, s, light, Point(1.1, 0, 0), camera, normal, false);

    EXPECT_TRUE(c1.isEqual(WHITE));
    EXPECT_TRUE(c2.isEqual(BLACK));
}

TEST(PatternTest, ApplyPattern_AppliesPatternWithObjectTransformation){
    Sphere* s = new Sphere;
    s->setTransform(scalingMatrix(2, 2, 2));
    Stripes* stripes = new Stripes;
    
    Colour c = stripes->applyPattern(s, Point(1.5, 0, 0));

    EXPECT_TRUE(c.isEqual(WHITE));
}

TEST(PatternTest, ApplyPattern_AppliesPatternWithPatternTransformation){
    Sphere* s = new Sphere;
    Stripes* stripes = new Stripes;
    stripes->setTransform(scalingMatrix(2, 2, 2));
    
    Colour c = stripes->applyPattern(s, Point(1.5, 0, 0));

    EXPECT_TRUE(c.isEqual(WHITE));
}

TEST(PatternTest, ApplyPattern_AppliesPatternWithObjectAndPatternTransformation){
    Sphere* s = new Sphere;
    s->setTransform(scalingMatrix(2, 2, 2));
    Stripes* stripes = new Stripes;
    stripes->setTransform(translationMatrix(0.5, 0, 0));
    
    Colour c = stripes->applyPattern(s, Point(2.5, 0, 0));

    EXPECT_TRUE(c.isEqual(WHITE));
}

TEST(LinearGradientTest, ChildApplyPattern_AppliesLinearGradientPattern){
    LinearGradient p;
    
    EXPECT_TRUE(p.ChildApplyPattern(Point()).isEqual(WHITE));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0.25, 0, 0)).isEqual(Colour(0.75, 0.75, 0.75)));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0.5, 0, 0)).isEqual(Colour(0.5, 0.5, 0.5)));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0.75, 0, 0)).isEqual(Colour(0.25, 0.25, 0.25)));
}

TEST(RingPatternTest, ChildApplyPattern_AppliesRingPatternPattern){
    RingPattern p;
    
    EXPECT_TRUE(p.ChildApplyPattern(Point()).isEqual(WHITE));
    EXPECT_TRUE(p.ChildApplyPattern(Point(1, 0, 0)).isEqual(BLACK));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0, 0, 1)).isEqual(BLACK));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0.708, 0, 0.708)).isEqual(BLACK));
}

TEST(CheckerPatternTest, ChildApplyPattern_AppliesCheckerPatternPattern){
    CheckerPattern p;
    
    EXPECT_TRUE(p.ChildApplyPattern(Point()).isEqual(WHITE));

    EXPECT_TRUE(p.ChildApplyPattern(Point(0.99, 0, 0)).isEqual(WHITE));
    EXPECT_TRUE(p.ChildApplyPattern(Point(1.01, 0, 0)).isEqual(BLACK));

    EXPECT_TRUE(p.ChildApplyPattern(Point(0, 0.99, 0)).isEqual(WHITE));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0, 1.01, 0)).isEqual(BLACK));

    EXPECT_TRUE(p.ChildApplyPattern(Point(0, 0, 0.99)).isEqual(WHITE));
    EXPECT_TRUE(p.ChildApplyPattern(Point(0, 0, 1.01)).isEqual(BLACK));
}