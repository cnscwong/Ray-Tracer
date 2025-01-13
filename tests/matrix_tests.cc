#include <gtest/gtest.h>
#include "Matrix.h"
#include "common.h"

TEST(MatrixTests, BasicTest){
    // Default constructor
    Matrix a;
    EXPECT_EQ(a.getRows(), DEFAULT_ROWS);
    EXPECT_EQ(a.getCols(), DEFAULT_COLS);

    // Constructor with matrix dims
    Matrix b(4, 5);
    EXPECT_EQ(b.getRows(), 4);
    EXPECT_EQ(b.getCols(), 5);

    // Invalid parameters, vector constructor
    std::vector<std::vector<float>> v = {{-3, 5}, {1}};
    Matrix c1(2, 2, v);
    EXPECT_TRUE(floatIsEqual(c1.getElement(0, 0), 0));
    EXPECT_TRUE(floatIsEqual(c1.getElement(0, 1), 0));
    EXPECT_TRUE(floatIsEqual(c1.getElement(1, 0), 0));
    EXPECT_TRUE(floatIsEqual(c1.getElement(1, 1), 0));

    std::vector<std::vector<float>> vec = {{-3, 5}, {1, -2}};
    Matrix c2(2, 2, vec);
    EXPECT_TRUE(floatIsEqual(c2.getElement(0, 0), -3));
    EXPECT_TRUE(floatIsEqual(c2.getElement(0, 1), 5));
    EXPECT_TRUE(floatIsEqual(c2.getElement(1, 0), 1));
    EXPECT_TRUE(floatIsEqual(c2.getElement(1, 1), -2));

    std::vector<std::vector<float>> vec1 = {{-3, 5, 0}, {1, -2, -7}, {0, 1, 1}};
    Matrix d(3, 3, vec1);
    EXPECT_EQ(d.getRows(), 3);
    EXPECT_EQ(d.getCols(), 3);
    EXPECT_TRUE(floatIsEqual(d.getElement(0, 0), -3));
    EXPECT_TRUE(floatIsEqual(d.getElement(1, 1), -2));
    EXPECT_TRUE(floatIsEqual(d.getElement(2, 2), 1));

    std::vector<std::vector<float>> vec2 = {{1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5}};
    Matrix e(4, 4, vec2);
    EXPECT_EQ(e.getRows(), 4);
    EXPECT_EQ(e.getCols(), 4);
    EXPECT_TRUE(floatIsEqual(e.getElement(0, 0), 1));
    EXPECT_TRUE(floatIsEqual(e.getElement(0, 3), 4));
    EXPECT_TRUE(floatIsEqual(e.getElement(1, 0), 5.5));
    EXPECT_TRUE(floatIsEqual(e.getElement(1, 2), 7.5));
    EXPECT_TRUE(floatIsEqual(e.getElement(2, 2), 11));
    EXPECT_TRUE(floatIsEqual(e.getElement(3, 0), 13.5));
    EXPECT_TRUE(floatIsEqual(e.getElement(3, 2), 15.5));
}

TEST(MatrixTests, MatrixEquality){
    std::vector<std::vector<float>> v1 = {{-3, 5}, {1, 0}};
    std::vector<std::vector<float>> v2 = {{-3, 5}, {1, 0}};
    Matrix c1(2, 2, v1);
    Matrix c2(2, 2, v2);
    EXPECT_TRUE(c1.isEqual(c2));

    std::vector<std::vector<float>> v3 = {{-3, 5}, {1, 1}};
    Matrix c3(2, 2, v3);
    EXPECT_FALSE(c1.isEqual(c3));
}

TEST(MatrixTests, MatrixMultiplicationSameDimensions){
    std::vector<std::vector<float>> v1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    std::vector<std::vector<float>> v2 = {{-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8}};
    Matrix a(4, 4, v1);
    Matrix b(4, 4, v2);

    std::vector<std::vector<float>> v3 = {{20, 22, 50, 48}, {44, 54, 114, 108}, {40, 58, 110, 102}, {16, 26, 46, 42}};
    Matrix c(4, 4, v3);

    EXPECT_TRUE(c.isEqual(a*b));
}

TEST(MatrixTests, MatrixMultiplicationDiffDimensions){
    std::vector<std::vector<float>> v1 = {{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}};
    std::vector<std::vector<float>> v2 = {{1}, {2}, {3}, {1}};
    Matrix a(4, 4, v1);
    Matrix b(4, 1, v2);

    std::vector<std::vector<float>> v3 = {{18}, {24}, {33}, {1}};
    Matrix c(4, 1, v3);
    EXPECT_TRUE(c.isEqual(a*b));
    Tuple d(1, 2, 3, 1);
    EXPECT_TRUE(Tuple(18, 24, 33, 1).isEqual(a*d));
}

TEST(MatrixTests, MatrixMultiplicationIdentity){
    std::vector<std::vector<float>> v1 = {{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}};
    Matrix a(4);
    Matrix b(4, 4, v1);

    std::vector<std::vector<float>> v3 = {{1}, {2}, {3}, {4}};
    Matrix c(4, 1, v3);
    EXPECT_TRUE(b.isEqual(b*a));
    EXPECT_TRUE(c.isEqual(a*c));
}

TEST(MatrixTests, MatrixTranspose){
    std::vector<std::vector<float>> v1 = {{0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8}};
    std::vector<std::vector<float>> v2 = {{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}};
    Matrix a(4, 4, v1);
    Matrix b(4, 4, v2);
    EXPECT_TRUE(b.isEqual(a.transpose()));
    EXPECT_TRUE(Matrix(4).isEqual(Matrix(4).transpose()));
}

TEST(MatrixTests, MatrixTwoByTwoDeterminant){
    std::vector<std::vector<float>> v1 = {{1, 5}, {-3, 2}};
    Matrix a(2, 2, v1);
    EXPECT_TRUE(floatIsEqual(a.determinant(), 17));
}

TEST(MatrixTests, Submatrix){
    std::vector<std::vector<float>> v1 = {{1, 5, 0}, {-3, 2, 7}, {0, 6, -3}};
    std::vector<std::vector<float>> v2 = {{-3, 2}, {0, 6}};
    Matrix a(3, 3, v1);
    EXPECT_TRUE(a.submatrix(0, 2).isEqual(Matrix(2, 2, v2)));

    std::vector<std::vector<float>> v3 = {{-6, 1, 1, 6}, {-8, 5, 8, 6}, {-1, 0, 8, 2}, {-7, 1, -1, 1}};
    std::vector<std::vector<float>> v4 = {{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}};
    Matrix b(4, 4, v3);
    EXPECT_TRUE(b.submatrix(2, 1).isEqual(Matrix(3, 3, v4)));
}

TEST(MatrixTests, MinorAndCofactor){
    std::vector<std::vector<float>> v1 = {{3, 5, 0}, {2, -1, -7}, {6, -1, 5}};
    Matrix a(3, 3, v1);
    EXPECT_TRUE(floatIsEqual(a.minor(0, 0), -12));
    EXPECT_TRUE(floatIsEqual(a.cofactor(0, 0), -12));
    EXPECT_TRUE(floatIsEqual(a.minor(1, 0), 25));
    EXPECT_TRUE(floatIsEqual(a.cofactor(1, 0), -25));
}

TEST(MatrixTests, MatrixDeterminant){
    std::vector<std::vector<float>> v1 = {{1, 2, 6}, {-5, 8, -4}, {2, 6, 4}};
    Matrix a(3, 3, v1);
    EXPECT_TRUE(floatIsEqual(a.cofactor(0, 0), 56));
    EXPECT_TRUE(floatIsEqual(a.cofactor(0, 1), 12));
    EXPECT_TRUE(floatIsEqual(a.cofactor(0, 2), -46));
    EXPECT_TRUE(floatIsEqual(a.determinant(), -196));

    std::vector<std::vector<float>> v2 = {{-2, -8, 3, 5}, {-3, 1, 7, 3}, {1, 2, -9, 6}, {-6, 7, 7, -9}};
    Matrix b(4, 4, v2);
    EXPECT_TRUE(floatIsEqual(b.cofactor(0, 0), 690));
    EXPECT_TRUE(floatIsEqual(b.cofactor(0, 1), 447));
    EXPECT_TRUE(floatIsEqual(b.cofactor(0, 2), 210));
    EXPECT_TRUE(floatIsEqual(b.cofactor(0, 3), 51));
    EXPECT_TRUE(floatIsEqual(b.determinant(), -4071));
}

TEST(MatrixTests, MatrixInverse){
    std::vector<std::vector<float>> v1 = {{6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6}};
    Matrix a(4, 4, v1);
    EXPECT_TRUE(a.isInvertable());

    std::vector<std::vector<float>> v2 = {{-4, 2, -2, -3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0}};
    Matrix b(4, 4, v2);
    EXPECT_FALSE(b.isInvertable());

    std::vector<std::vector<float>> v3 = {{-5, 2, 6, -8}, {1, -5, 1, 8}, {7, 7, -6, -7}, {1, -3, 7, 4}};
    std::vector<std::vector<float>> v4 = {{0.21805, 0.45113, 0.24060, -0.04511}, {-0.80827, -1.45677, -0.44361, 0.52068}, {-0.07895, -0.22368, -0.05263, 0.19737}, {-0.52256, -0.81391, -0.30075, 0.30639}};
    Matrix c(4, 4, v3);
    Matrix d(4, 4, v4);
    EXPECT_TRUE(c.inverse().isEqual(d));
}

TEST(MatrixTests, MoreMatrixInverse){
    std::vector<std::vector<float>> v1 = {{8, -5, 9, 2}, {7, 5, 6, 1}, {-6, 0, 9, 6}, {-3, 0, -9, -4}};
    std::vector<std::vector<float>> v2 = {{-0.15385, -0.15385, -0.28205, -0.53846}, {-0.07692, 0.12308, 0.02564, 0.03077}, {0.35897, 0.35897, 0.43590, 0.92308}, {-0.69231, -0.69231, -0.76923, -1.92308}};
    Matrix a(4, 4, v1);
    Matrix b(4, 4, v2);
    EXPECT_TRUE(a.inverse().isEqual(b));

    std::vector<std::vector<float>> v3 = {{9, 3, 0, 9}, {-5, -2, -6, -3}, {-4, 9, 6, 4}, {-7, 6, 6, 2}};
    std::vector<std::vector<float>> v4 = {{-0.04074, -0.07778, 0.14444, -0.22222}, {-0.07778, 0.03333, 0.36667, -0.33333}, {-0.02901, -0.14630, -0.10926, 0.12963}, {0.17778, 0.06667, -0.26667, 0.33333}};
    Matrix c(4, 4, v3);
    Matrix d(4, 4, v4);
    EXPECT_TRUE(c.inverse().isEqual(d));

    std::vector<std::vector<float>> v5 = {{3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 1}, {-6, 5, -1, 1}};
    std::vector<std::vector<float>> v6 = {{8, 2, 2, 2}, {3, -1, 7, 0}, {7, 0, 5, 4}, {6, -2, 0, 5}};
    Matrix e(4, 4, v5);
    Matrix f(4, 4, v6);
    EXPECT_TRUE(((e*f)*f.inverse()).isEqual(e));
}

TEST(MatrixTransformations, TranslationTest){
    Matrix transform = translationMatrix(5, -3, 2);
    Point p(-3, 4, 5);
    EXPECT_TRUE((transform*p).isEqual(Point(2, 1, 7)));
    EXPECT_TRUE((transform.inverse()*p).isEqual(Point(-8, 7, 3)));
    
    // Testing that multiplication of translation matrix with vector does nothing
    Vector v(-3, 4, 5);
    EXPECT_TRUE(v.isEqual(transform*v));
}

TEST(MatrixTransformations, ScalingTest){
    Matrix transform = scalingMatrix(2, 3, 4);
    Point p(-4, 6, 8);
    EXPECT_TRUE((transform*p).isEqual(Point(-8, 18, 32)));

    Vector v(-4, 6, 8);
    EXPECT_TRUE((transform*v).isEqual(Vector(-8, 18, 32)));
    EXPECT_TRUE((transform.inverse()*v).isEqual(Vector(-2, 2, 2)));

    // Reflection Test
    transform = scalingMatrix(-1, 1, 1);
    p = Point(2, 3, 4);
    EXPECT_TRUE((transform*p).isEqual(Point(-2, 3, 4)));
}

TEST(MatrixTransformations, RotationTest){
    Point p(0, 1, 0);
    Vector v(0, 1, 0);
    Matrix half_quarter = xRotationMatrix(PI/4);
    Matrix full_quarter = xRotationMatrix(PI/2);
    EXPECT_TRUE((half_quarter*p).isEqual(Point(0, sqrt(2)/2, sqrt(2)/2)));
    EXPECT_TRUE((full_quarter*p).isEqual(Point(0, 0, 1)));
    EXPECT_TRUE((half_quarter.inverse()*p).isEqual(Point(0, sqrt(2)/2, -sqrt(2)/2)));
    EXPECT_TRUE((half_quarter*v).isEqual(Vector(0, sqrt(2)/2, sqrt(2)/2)));
    EXPECT_TRUE((full_quarter*v).isEqual(Vector(0, 0, 1)));
    EXPECT_TRUE((half_quarter.inverse()*v).isEqual(Vector(0, sqrt(2)/2, -sqrt(2)/2)));

    p = Point(0, 0, 1);
    half_quarter = yRotationMatrix(PI/4);
    full_quarter = yRotationMatrix(PI/2);
    EXPECT_TRUE((half_quarter*p).isEqual(Point(sqrt(2)/2, 0, sqrt(2)/2)));
    EXPECT_TRUE((full_quarter*p).isEqual(Point(1, 0, 0)));

    p = Point(0, 1, 0);
    half_quarter = zRotationMatrix(PI/4);
    full_quarter = zRotationMatrix(PI/2);
    EXPECT_TRUE((half_quarter*p).isEqual(Point(-sqrt(2)/2, sqrt(2)/2, 0)));
    EXPECT_TRUE((full_quarter*p).isEqual(Point(-1, 0, 0)));
}

TEST(MatrixTransformations, ShearingTest){
    Point p(2, 3, 4);
    Matrix transform = shearingMatrix(1, 0, 0, 0, 0, 0);
    EXPECT_TRUE((transform*p).isEqual(Point(5, 3, 4)));
    transform = shearingMatrix(0, 1, 0, 0, 0, 0);
    EXPECT_TRUE((transform*p).isEqual(Point(6, 3, 4)));
    transform = shearingMatrix(0, 0, 1, 0, 0, 0);
    EXPECT_TRUE((transform*p).isEqual(Point(2, 5, 4)));
    transform = shearingMatrix(0, 0, 0, 1, 0, 0);
    EXPECT_TRUE((transform*p).isEqual(Point(2, 7, 4)));
    transform = shearingMatrix(0, 0, 0, 0, 1, 0);
    EXPECT_TRUE((transform*p).isEqual(Point(2, 3, 6)));
    transform = shearingMatrix(0, 0, 0, 0, 0, 1);
    EXPECT_TRUE((transform*p).isEqual(Point(2, 3, 7)));
}

TEST(MatrixTransformations, ChainingTest){
    Point p(1, 0, 1);
    Matrix A = xRotationMatrix(PI/2);
    Matrix B = scalingMatrix(5, 5, 5);
    Matrix C = translationMatrix(10, 5, 7);
    Tuple p2 = A*p;
    Tuple p3 = B*p2;
    Tuple p4 = C*p3;

    EXPECT_TRUE(p2.isEqual(Point(1, -1, 0)));
    EXPECT_TRUE(p3.isEqual(Point(5, -5, 0)));
    EXPECT_TRUE(p4.isEqual(Point(15, 0, 7)));

    Matrix transform = chainTransformationMatrices({A, B, C});
    EXPECT_TRUE((transform*p).isEqual(p4));
    EXPECT_TRUE(transform.isEqual((C*B*A)));
}