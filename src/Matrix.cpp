#include "Matrix.h"

// Matrix constructors
Matrix::Matrix(){
    rows = DEFAULT_COLS;
    cols = DEFAULT_ROWS;

    matrix = std::vector<std::vector<float>>(rows, std::vector<float> (cols, 0));
}

// Constructor to make ixi identity matrix
Matrix::Matrix(int i){
    rows = i;
    cols = i;

    matrix = std::vector<std::vector<float>>(rows, std::vector<float> (cols, 0));

    for(int a = 0; a < i; a++){
        matrix[a][a] = 1;
    }
}

// Constructor with given rows and columns
Matrix::Matrix(int r, int c){
    if(r < 1){
        rows = DEFAULT_ROWS;
    }else{
        rows = r;
    }
    
    if(c < 1){
        cols = DEFAULT_COLS;
    }else{
        cols = c;
    }

    matrix = std::vector<std::vector<float>>(rows, std::vector<float> (cols, 0));
}

// Constructor with preset vector
Matrix::Matrix(int r, int c, std::vector<std::vector<float>> vec){
    rows = r;
    cols = c;
    
    bool valid = true;
    if(vec.size() == rows){
        for(int i = 0; i < rows; i++){
            if(vec.at(i).size() != cols){
                valid = false;
                break;
            }
        }
    }else{
        valid = false;
    }

    if(valid){
        matrix = vec;
    }else{
        matrix = std::vector<std::vector<float>>(rows, std::vector<float> (cols, 0));
    }
}

// Checks if given xy coordinates are within range of the matrix dimensions
bool Matrix::checkCoordValid(int x, int y){
    if(x > -1 && y > -1 && x < rows && y < cols){
        return true;
    }
    return false;
}

// Getter for rows
int Matrix::getRows(){
    return rows;
}

// Getter for cols
int Matrix::getCols(){
    return cols;
}

// Getter for element at coord xy
float Matrix::getElement(int x, int y){
    if(this->checkCoordValid(x, y)){
        return matrix.at(x).at(y);
    }else{
        throw std::invalid_argument("getElement: received invalid xy coordinates [" + std::to_string(x) + ", " + std::to_string(y) + "]");
    }
}

// Sets element in matrix at coord xy to val
void Matrix::setElement(int x, int y, float val){
    if(this->checkCoordValid(x, y)){
        matrix[x][y] = val;
    }else{
        throw std::invalid_argument("setElement: received invalid xy coordinates [" + std::to_string(x) + ", " + std::to_string(y) + "]");
    }
}

// Matrix getter
std::vector<std::vector<float>> Matrix::getMatrix(){
    return matrix;
}

// Matrix equality check
bool Matrix::isEqual(Matrix a){
    if(rows != a.getRows() || cols != a.getCols()){
        return false;
    }

    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if(!floatIsEqual(this->getElement(r, c), a.getElement(r, c))){
                return false;
            }
        }
    }
    return true;
}

// Matrix multiplication
Matrix multiplyMatrix(Matrix m1, Matrix m2){
    // Invalid matrix multiplication
    if(m1.getCols() != m2.getRows()){
        throw std::invalid_argument("Invalid matrix dimensions");
    }

    // Initializes matrix result
    Matrix m(m1.getRows(), m2.getCols());
    float s = 0;
    // Iterates through all elements and sets values in matrix result
    for(int r = 0; r < m.getRows(); r++){
        for(int c = 0; c < m.getCols(); c++){
            s = 0;
            // Calculating value at coord rc
            for(int i = 0; i < m1.getCols(); i++){
                s += m1.getElement(r, i)*m2.getElement(i, c);
            }
            // Sets value s at coordinate rc
            m.setElement(r, c, s);
        }
    }

    return m;
}

// Matrix multiplication with vector
Matrix multiplyMatrixTuple(Matrix m1, Tuple m2){
    // Invalid matrix multiplication
    if(m1.getCols() != 4){
        throw std::invalid_argument("multiplyMatrixTuple: Invalid matrix dimensions");
    }

    // Initializes matrix result
    Matrix m(m1.getRows(), 1);
    float s = 0;
    // Iterates through all elements and sets values in matrix result
    for(int r = 0; r < m.getRows(); r++){
        s = 0;
        s += m1.getElement(r, 0)*m2.x;
        s += m1.getElement(r, 1)*m2.y;
        s += m1.getElement(r, 2)*m2.z;
        s += m1.getElement(r, 3)*m2.point;
        // Sets value s at coordinate r, 0
        m.setElement(r, 0, s);
    }

    return m;
}

// Computes the transpose of the matrix
Matrix Matrix::transpose(){
    Matrix m(cols, rows);

    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            m.setElement(c, r, this->getElement(r, c));
        }
    }

    return m;
}

// Computes determinant for a 2x2 matrix
float Matrix::twoDet(){
    if(rows != 2 || cols != 2){
        throw std::invalid_argument("twoDet: Invalid matrix dimensions");
    }

    float det = 0;
    det += this->getElement(0, 0)*this->getElement(1, 1);
    det -= this->getElement(0, 1)*this->getElement(1, 0);
    return det;
}

// Computes submatrix that is generated when removing the xth row and yth column(For computing determinants)
Matrix Matrix::submatrix(int x, int y){
    if(!this->checkCoordValid(x, y)){
        throw std::invalid_argument("submatrix: Invalid xy coords");
    }else if(rows == 1 && cols == 1){
        throw std::invalid_argument("submatrix: matrix cannot be reduced");
    }

    std::vector<std::vector<float>> m = matrix;
    m.erase(m.begin() + x);
    for(int i = 0; i < m.size(); i++){
        m.at(i).erase(m.at(i).begin() + y);
    }

    return Matrix(rows - 1, cols - 1, m);
}

// Computes the minor of the matrix given coords xy
// Minor is defined as the determinant of the submatrix without row x and col y
float Matrix::minor(int x, int y){
    if(rows != cols || rows == 1 || rows == 2){
        throw std::invalid_argument("minor: matrix dimensions invalid");
    }

    return this->submatrix(x, y).determinant();
}

// Computes the cofactor of the matrix given coords xy
// Cofactor is the minor but negated if x + y == odd number
float Matrix::cofactor(int x, int y){
    if((x + y) % 2 == 1){
        return -1*this->minor(x, y);
    }else{
        return this->minor(x, y);
    }
}

// Calculates determinant of matrix
float Matrix::determinant(){
    if(rows != cols || rows == 1){
        throw std::invalid_argument("determinant: Matrix too small or not square matrix");
    }

    if(rows == 2 && cols == 2){
        return this->twoDet();
    }else{
        float det = 0;
        // Calculating determinant which is the sum of (every element in any row or col*cofactor of each element)
        for(int c = 0; c < cols; c++){
            det += getElement(0, c)*cofactor(0, c);
        }

        return det;
    }
}

// Checks if the matrix is invertable
bool Matrix::isInvertable(){
    // Checks if matrix is square and not 1x1
    if(rows != cols || rows == 1){
        return false;
    }

    return !floatIsEqual(this->determinant(), 0);
}

// Computes inverse of matrix
// Element [r, c] = cofactor(c, r)/det
Matrix Matrix::inverse(){
    if(!this->isInvertable()){
        throw std::invalid_argument("inverse: Matrix is not invertable");
    }

    Matrix m(rows, cols);
    float det = this->determinant();

    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            m.setElement(r, c, this->cofactor(c, r)/det);
        }
    }

    return m;
}