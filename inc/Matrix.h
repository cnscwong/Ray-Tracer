#pragma once
#include <vector>
#include "common.h"
#include <stdexcept>
#include "Tuple.h"
#include <string>
#include <cmath>

const int DEFAULT_ROWS = 2;
const int DEFAULT_COLS = 2;
const float PI = 3.14159265358979323846f;

class Matrix{
private:
    int rows;
    int cols;
    std::vector<std::vector<float>> matrix;
public:
    // Constructors
    Matrix();
    Matrix(int i);
    Matrix(int r, int c);
    Matrix(int r, int c, std::vector<std::vector<float>> vec);

    // Checks if given coordinates are valid
    bool checkCoordValid(int x, int y);

    // Getters and setters for variables
    int getRows();
    int getCols();
    float getElement(int x, int y);
    void setElement(int x, int y, float val);
    std::vector<std::vector<float>> getMatrix();

    // Equality check function
    bool isEqual(Matrix a);

    // Matrix operations
    // Transpose of matrix
    Matrix transpose();
    // Determinant for 2x2 matrix
    float twoDet();
    // Returns a submatrix with row x and col y removed
    Matrix submatrix(int x, int y);
    // Calculates the minor of the matrix given xy(More info in Matrix.cpp)
    float minor(int x, int y);
    // Calculates the cofactor of the matrix given xy(More info in Matrix.cpp)
    float cofactor(int x, int y);
    // Calculates the determinant
    float determinant();
    // Checks if matrix is invertable
    bool isInvertable();
    // Computes inverse of matrix
    Matrix inverse();

    Matrix operator*(Matrix m2);
    Tuple operator*(Tuple m2);
};

// Multiplies matrices together
Matrix multiplyMatrix(Matrix m1, Matrix m2);
// Multiplies a matrix by a tuple(4x1 matrix)
Tuple multiplyMatrixTuple(Matrix m1, Tuple m2);

// Matrix transformations
// Generates a translation matrix given x, y, z coordinates
Matrix translationMatrix(int x, int y, int z);
// Generates a scaling matrix given x, y, z coordinates
Matrix scalingMatrix(int x, int y, int z);
// Basic Rotations, rotates r radians around the specified axis in the function
Matrix xRotationMatrix(float r);
Matrix yRotationMatrix(float r);
Matrix zRotationMatrix(float r);
// Shearing matrix, x_y = x moved in proportion to y
Matrix shearingMatrix(float x_y, float x_z, float y_x, float y_z, float z_x, float z_y);
// Chaining transformations, input the matrix transformations as parameters to produce
// a matrix that performs all transformations at once when multiplied
Matrix chainTransformationMatrices(std::initializer_list<Matrix> matrices);