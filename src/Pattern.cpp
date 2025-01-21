#include "Pattern.h"
#include "Shape.h"

Matrix Pattern::getTransform(){
    return transform;
}

void Pattern::setTransform(Matrix m){
    transform = m;
}

Colour Pattern::applyPattern(Shape* s, Point p){
    // Transform the pattern based on how the object is transformed
    Point object_point = Point(s->getTransform().inverse()*p);
    // Transform the point based on how we want the pattern to be transformed
    Point pattern_point = Point(transform.inverse()*object_point);
    return ChildApplyPattern(pattern_point);
}

Colour Pattern::ChildApplyPattern(Point p){
    return Colour(p.x, p.y, p.z);
}

Stripes::Stripes(){
    stripes = std::vector<Colour>({WHITE, BLACK});
}

Stripes::Stripes(std::initializer_list<Colour> colours){
    stripes = std::vector<Colour>(colours);
}

Colour Stripes::ChildApplyPattern(Point p){
    return stripes.at((int)floor(p.x) % stripes.size());
}
