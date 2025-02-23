#include "CSG.h"

CSG::CSG(SetOperation op, Shape* l, Shape* r){
    this->op = op;
    left = l;
    right = r;
    l->setParent(this);
    r->setParent(this);
}

Shape* CSG::getLeft(){
    return left;
}

Shape* CSG::getRight(){
    return right;
}

SetOperation CSG::getOp(){
    return op;
}

bool CSG::validIntersection(bool hitLeft, bool insideLeft, bool insideRight){
    if(op == UNION){
        return (hitLeft && !insideRight) || (!hitLeft && !insideLeft);
    }else if(op == INTERSECTION){
        return (hitLeft && insideRight) || (!hitLeft && insideLeft);
    }else{ // set difference
        return (hitLeft && !insideRight) || (!hitLeft && insideLeft);
    }

    // This should never be hit
    throw std::invalid_argument("CSG operation is invalid");
}

std::vector<Intersection> CSG::filterIntersections(std::vector<Intersection> intersects){
    std::vector<Intersection> result;

    // Ray starts outside of both shapes
    bool insideLeft = false;
    bool insideRight = false;

    for(int i = 0; i < intersects.size(); i++){
        // Checks which shape the ray hits(left or right)
        bool hitLeft = left->includes(intersects.at(i).getShape());

        if(validIntersection(hitLeft, insideLeft, insideRight)){
            result.push_back(intersects.at(i));
        }

        // Ray is entering or exiting the shape that it hit(left or right)
        if(hitLeft){
            insideLeft = !insideLeft;
        }else{
            insideRight = !insideRight;
        }
    }

    return result;
}

bool CSG::includes(Shape* s){
    return left->includes(s) || right->includes(s);
}

std::vector<Intersection> CSG::childIntersections(Ray r){
    std::vector<Intersection> leftIntersects = left->findIntersections(r);
    std::vector<Intersection> rightIntersects = right->findIntersections(r);

    // Concatenates intersections of left and right shapes into one vector
    std::vector<Intersection> intersects;
    intersects.insert(intersects.end(), leftIntersects.begin(), leftIntersects.end());
    intersects.insert(intersects.end(), rightIntersects.begin(), rightIntersects.end());

    // Sorts concatenated vector
    std::sort(intersects.begin(), intersects.end(), compareIntersections);

    return filterIntersections(intersects);
}