// Parent class for points and vectors
class Tuple{
    public:
        float x, y, z;
        float point;

        Tuple(float x, float y, float z, float point);
        bool isEqual(Tuple a);
};

// Tuple Operations
Tuple addTuples(Tuple a, Tuple b);
Tuple subtractTuples(Tuple a, Tuple b);
Tuple negateTuple(Tuple a);
Tuple multiplyTuple(Tuple a, float scale);
Tuple divideTuple(Tuple a, float scale);

// Class for a point, inherits from Tuple
class Point : public Tuple{
    public:
        Point(float x, float y, float z);
};

// Class for a vector, inherits from Tuple
class Vector : public Tuple{
    public:
        Vector(float x, float y, float z);

        // Vector Operations
        float magnitude();
        void normalize();
};

// More Vector Operations
float dotProduct(Vector a, Vector b);
Vector crossProduct(Vector a, Vector b);
