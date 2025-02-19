#include "Group.h"

Group::Group(){
    name = "";
}

Group::Group(std::string n){
    name = n;
}

std::vector<Shape*> Group::getShapes(){
    return shapes;
}

void Group::appendShape(Shape* s){
    shapes.push_back(s);
    s->setParent(this);
}

std::vector<Intersection> Group::childIntersections(Ray r){
    std::vector<Intersection> intersects;
    std::vector<Intersection> temp;

    for(int i = 0; i < shapes.size(); i++){
        temp = shapes.at(i)->findIntersections(r);
        for(int j = 0; j < temp.size(); j++){
            intersects.push_back(temp.at(j));
        }
    }

    std::sort(intersects.begin(), intersects.end(), compareIntersections);
    return intersects;
}