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
Matrix Matrix::operator*(Matrix m2){
    // Invalid matrix multiplication
    if(cols != m2.getRows()){
        throw std::invalid_argument("Invalid matrix dimensions");
    }

    // Initializes matrix result
    Matrix m(rows, m2.getCols());
    float s = 0;
    // Iterates through all elements and sets values in matrix result
    for(int r = 0; r < m.getRows(); r++){
        for(int c = 0; c < m.getCols(); c++){
            s = 0;
            // Calculating value at coord rc
            for(int i = 0; i < cols; i++){
                s += getElement(r, i)*m2.getElement(i, c);
            }
            // Sets value s at coordinate rc
            m.setElement(r, c, s);
        }
    }

    return m;
}

// Matrix multiplication with vector
Tuple Matrix::operator*(Tuple m2){
    // Invalid matrix multiplication
    if(cols != 4){
        throw std::invalid_argument("multiplyMatrixTuple: Invalid matrix dimensions");
    }

    // Initializes matrix result
    Matrix m(rows, 1);
    float s = 0;
    // Iterates through all elements and sets values in matrix result
    for(int r = 0; r < m.getRows(); r++){
        s = 0;
        s += getElement(r, 0)*m2.x;
        s += getElement(r, 1)*m2.y;
        s += getElement(r, 2)*m2.z;
        s += getElement(r, 3)*m2.point;
        // Sets value s at coordinate r, 0
        m.setElement(r, 0, s);
    }

    return Tuple(m.getElement(0, 0), m.getElement(1, 0), m.getElement(2, 0), m.getElement(3, 0));
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

// Computes translation matrix given x, y, and z
// When this matrix is multiplied with a Point
// The point will be translated in the x direction
// x units away, the y direction y units away, and 
// the z direction z units away
// Additionally, multiplying a Vector by this matrix will
// do nothing because of the point variable being 0.0
Matrix translationMatrix(float x, float y, float z){
    // Generates 4x4 identity matrix
    Matrix m(4);

    // Translation matrix is equivalent to:
    // 1 0 0 x
    // 0 1 0 y
    // 0 0 1 z
    // 0 0 0 1

    m.setElement(0, 3, x);
    m.setElement(1, 3, y);
    m.setElement(2, 3, z);
    return m;
}

// Does same thing as translation matrix but the multiplied tuple
// is scaled instead. Works for both points and vectors
// Can also perform reflection using negative input parameters
Matrix scalingMatrix(float x, float y, float z){
    // Generates 4x4 identity matrix
    Matrix m(4);

    // Scaling matrix is equivalent to:
    // x 0 0 0
    // 0 y 0 0
    // 0 0 z 0
    // 0 0 0 1

    m.setElement(0, 0, x);
    m.setElement(1, 1, y);
    m.setElement(2, 2, z);
    return m;
}

// Rotation functions
// Does same thing as translation matrix but the multiplied tuple
// is rotated along specified axis in funct name instead. Works 
// for points and vectors
Matrix xRotationMatrix(float r){
    // Generates 4x4 identity matrix
    Matrix m(4);

    // Scaling matrix is equivalent to:
    // 1  0      0      0
    // 0 cos(r) -sin(r) 0
    // 0 sin(r) cos(r)  0
    // 0  0      0      1

    m.setElement(1, 1, cos(r));
    m.setElement(1, 2, -sin(r));
    m.setElement(2, 1, sin(r));
    m.setElement(2, 2, cos(r));
    return m;
}

Matrix yRotationMatrix(float r){
    // Generates 4x4 identity matrix
    Matrix m(4);

    // Scaling matrix is equivalent to:
    // cos(r)  0 sin(r) 0
    // 0       1  0     0
    // -sin(r) 0 cos(r) 0
    // 0       0  0     1

    m.setElement(0, 0, cos(r));
    m.setElement(0, 2, sin(r));
    m.setElement(2, 0, -sin(r));
    m.setElement(2, 2, cos(r));
    return m;
}

Matrix zRotationMatrix(float r){
    // Generates 4x4 identity matrix
    Matrix m(4);

    // Scaling matrix is equivalent to:
    // cos(r) -sin(r) 0 0
    // sin(r)  cos(r) 0 0
    //  0        0    1 0
    //  0        0    0 1

    m.setElement(0, 0, cos(r));
    m.setElement(0, 1, -sin(r));
    m.setElement(1, 0, sin(r));
    m.setElement(1, 1, cos(r));
    return m;
}

// For x_y, x changes in proportion to y, the farther the y coordinate of the point is from 0,
// the more the x value changes. This can be applied to x and z too(x_z) and other axis combinations
// as seen in the parameters. This has the effect of making a straight line slanted, etc. Meant to be 
// used for points, although vectors would work
Matrix shearingMatrix(float x_y, float x_z, float y_x, float y_z, float z_x, float z_y){
    // Generates 4x4 identity matrix
    Matrix m(4);

    // Scaling matrix is equivalent to:
    // 1   x_y x_z 0
    // y_x 1   y_z 0
    // z_x z_y 1   0
    // 0   0   0   1

    m.setElement(0, 1, x_y);
    m.setElement(0, 2, x_z);
    m.setElement(1, 0, y_x);
    m.setElement(1, 2, y_z);
    m.setElement(2, 0, z_x);
    m.setElement(2, 1, z_y);
    return m;
}

Matrix chainTransformationMatrices(std::initializer_list<Matrix> matrices){
    Matrix result = Matrix(4);
    for (auto m : matrices) {
        result = m*result;
    }

    // Returns multiplication of current matrix with rest of the matrices
    return result;
}

// View transformation matrix. Moves the world relative to the camera. Intuitively, you can think of it as moving
//  the "camera" around the world to view it from different positions/directions. The eyePosition parameter is the 
// point where the camera is located. The to parameter is where the camera is looking. The up parameter specifies 
// which direction is pointing upwards from the camera

// Compute the "forward" vector(to - eyePosition) that points from the camera to the point that we want to view
// Compute the vector "left" which is the cross product of forward and the normalized up vector
// Up is an approximation, so you can recompute the "trueUp" vector relative to the eyeToPoint and 
// left vectors by calculating the cross product of those two vectors
// The resulting matrix is a 4x4 matrix that looks like this
// left.x     left.y     left.z     0
// trueUp.x   trueUp.y   trueUp.z   0
// -forward.x -forward.y -forward.z 0
// 0          0          0          1

// Afterwards, multiply this matrix by the translationMatrix(-eyePosition). This is because since you are actually
// moving the world relative to the camera, you need to orient the scene and then move it to the appropriate position
// relative to the camera
Matrix viewTransformationMatrix(Point eyePosition, Point to, Vector up){
    Vector forward = Vector((to - eyePosition)).normalize();
    Vector left = crossProduct(forward, up.normalize());
    Vector trueUp = crossProduct(left, forward);
    Matrix orientation(4);

    orientation.setElement(0, 0, left.x);
    orientation.setElement(0, 1, left.y);
    orientation.setElement(0, 2, left.z);    

    orientation.setElement(1, 0, trueUp.x);
    orientation.setElement(1, 1, trueUp.y);
    orientation.setElement(1, 2, trueUp.z);

    orientation.setElement(2, 0, -forward.x);
    orientation.setElement(2, 1, -forward.y);
    orientation.setElement(2, 2, -forward.z);

    return (orientation*translationMatrix(-eyePosition.x, -eyePosition.y, -eyePosition.z));
}