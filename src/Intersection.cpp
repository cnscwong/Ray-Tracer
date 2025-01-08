#include "Intersection.h"

// Intersection constructor
Intersection::Intersection(float t, Sphere s){
    time = t;
    this->s = s;
}

// Getters for Intersection variables
float Intersection::getTime(){
    return time;
}

Sphere Intersection::getSphere(){
    return s;
}

// Aggregating intersections into a vector
std::vector<Intersection> intersections(std::initializer_list<Intersection> l){
    return std::vector<Intersection>(l);
}

// Finds the intersection with the lowest non-negative time in the vector
int hit(std::vector<Intersection> vec){
    int ind = -1;
    float t = INFINITY;
    for(int i = 0; i < vec.size(); i++){
        if(vec.at(i).getTime() >= 0 && vec.at(i).getTime() < t){
            ind = i;
            t = vec.at(i).getTime();
        }
    }

    return ind;
}