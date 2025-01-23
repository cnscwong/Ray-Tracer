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
    colours = std::vector<Colour>({WHITE, BLACK});
}

Stripes::Stripes(std::initializer_list<Colour> colours){
    this->colours = std::vector<Colour>(colours);

    if(colours.size() == 0){
        this->colours = std::vector<Colour>({WHITE, BLACK});
    }
}

Colour Stripes::ChildApplyPattern(Point p){
    return colours.at((int)floor(p.x) % colours.size());
}

LinearGradient::LinearGradient(){
    colours = std::vector<Colour>({WHITE, BLACK});
}

LinearGradient::LinearGradient(std::initializer_list<Colour> colours){
    if(colours.size() == 2){
        this->colours = std::vector<Colour>(colours);
    }else{
        this->colours = std::vector<Colour>({WHITE, BLACK});
    }
}

Colour LinearGradient::ChildApplyPattern(Point p){
    Colour colourDistance = colours.at(1) - colours.at(0);
    float pointDistance = p.x - (float)floor(p.x);

    return colours.at(0) + colourDistance*pointDistance;
}

RingPattern::RingPattern(){
    colours = std::vector<Colour>({WHITE, BLACK});
}

RingPattern::RingPattern(std::initializer_list<Colour> colours){
    this->colours = std::vector<Colour>(colours);

    if(colours.size() == 0){
        this->colours = std::vector<Colour>({WHITE, BLACK});
    }
}

Colour RingPattern::ChildApplyPattern(Point p){
    return colours.at((int)floor(sqrt(pow(p.x, 2) + pow(p.z, 2))) % colours.size());
}

CheckerPattern::CheckerPattern(){
    colours = std::vector<Colour>({WHITE, BLACK});
}

CheckerPattern::CheckerPattern(std::initializer_list<Colour> colours){
    this->colours = std::vector<Colour>(colours);

    if(colours.size() == 0){
        this->colours = std::vector<Colour>({WHITE, BLACK});
    }
}

Colour CheckerPattern::ChildApplyPattern(Point p){
    return colours.at((int)floor(floor(p.x) + floor(p.y) + floor(p.z)) % colours.size());
}
