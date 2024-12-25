#pragma once
#include <vector>
#include "common.h"
#include <stdexcept>
#include "Tuple.h"
#include <string>

const int DEFAULT_ROWS = 2;
const int DEFAULT_COLS = 2;

class Matrix{
private:
    int rows;
    int cols;
    std::vector<std::vector<float>> matrix;
public:    
    Matrix();
    Matrix(int i);
    Matrix(int r, int c);
    Matrix(int r, int c, std::vector<std::vector<float>> vec);

    bool checkCoordValid(int x, int y);

    int getRows();
    int getCols();
    float getElement(int x, int y);
    void setElement(int x, int y, float val);
    std::vector<std::vector<float>> getMatrix();
    bool isEqual(Matrix a);
    Matrix transpose();
    float twoDet();
    Matrix submatrix(int x, int y);
    float minor(int x, int y);
    float cofactor(int x, int y);
    float determinant();
    bool isInvertable();
    Matrix inverse();
};

Matrix multiplyMatrix(Matrix m1, Matrix m2);
Matrix multiplyMatrixTuple(Matrix m1, Tuple m2);