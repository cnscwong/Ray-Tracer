// Parent class for points and vectors
class Tuple{
    public:
        float x, y, z;
        float point;

        Tuple(float x, float y, float z, float point);
        float isPoint();
};

// Class for a point, inherits from Tuple
class Point : public Tuple{
    public:
        Point(float x, float y, float z);
};

// Class for a vector, inherits from Tuple
class Vector : public Tuple{
    public:
        Vector(float x, float y, float z);
};
