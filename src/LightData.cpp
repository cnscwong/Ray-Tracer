#include "LightData.h"

// Light data constructors
LightData::LightData(){
    object = Sphere();
    time = 0;
    point = Point();
    camera = Vector();
    normal = Vector();
    insideObject = false;
}

LightData::LightData(Sphere o, float t, Point p, Vector e, Vector n){
    object = o;
    time = t;
    point = p;
    camera = e;
    normal = n;
    insideObject = false;
}

// Packs the data required for the computeLighting function into the LightData data structure
LightData prepareLightData(Intersection i, Ray r){
    LightData data;

    data.time = i.getTime();
    data.object = i.getSphere();

    data.point = r.computePosition(data.time);
    data.camera = Vector(r.getDirection().negateTuple());
    data.normal = data.object.computeNormal(data.point);

    if(dotProduct(data.normal, data.camera) < 0){
        data.insideObject = true;
        data.normal = Vector(data.normal.negateTuple());
    }

    return data;
}