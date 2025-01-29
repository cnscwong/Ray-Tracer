#include "LightData.h"

// Light data constructors
LightData::LightData(){
    object = new Sphere;
    time = 0;
    point = Point();
    camera = Vector();
    normal = Vector();
    reflect = Vector();
    insideObject = false;
    overPoint = point + normal*EPSILON;
}

LightData::LightData(Shape* o, float t, Point p, Vector e, Vector n){
    object = o;
    time = t;
    point = p;
    camera = e;
    normal = n;
    insideObject = false;
    overPoint = point + normal*EPSILON;
}

// Packs the data required for the computeLighting function into the LightData data structure
LightData prepareLightData(Intersection i, Ray r){
    LightData data;

    data.time = i.getTime();
    data.object = i.getShape();

    data.point = r.computePosition(data.time);
    data.camera = Vector(r.getDirection().negateTuple());
    data.normal = data.object->computeNormal(data.point);

    if(dotProduct(data.normal, data.camera) < 0){
        data.insideObject = true;
        data.normal = Vector(data.normal.negateTuple());
    }

    data.reflect = reflectVector(r.getDirection(), data.normal);
    data.overPoint = data.point + data.normal*EPSILON;
    return data;
}