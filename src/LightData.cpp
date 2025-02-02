#include "LightData.h"

// Light data constructor
LightData::LightData(){
    object = new Sphere;
    time = 0;
    point = Point();
    camera = Vector();
    normal = Vector();
    reflect = Vector();
    insideObject = false;
    overPoint = point + normal*EPSILON;
    n1 = 1;
    n2 = 1;
}

// Packs the data required for the computeLighting function into the LightData data structure
LightData prepareLightData(Intersection i, Ray r, std::vector<Intersection> rayIntersects){
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
    data.underPoint = data.point - data.normal*EPSILON;

    findRefractiveIndices(data, i, rayIntersects);

    return data;
}

// Algorithm for computing the refractive indices of the material being exited and the material being entered
void findRefractiveIndices(LightData &data, Intersection i, std::vector<Intersection> rayIntersects){
    std::vector<Shape*> containers;

    for(int a = 0; a < rayIntersects.size(); a++){
        // if the current index is the hit
        if(rayIntersects.at(a).isEqual(i)){
            // Ray is not currently in any object
            if(containers.empty()){
                data.n1 = 1.0;
            }else{
                // n1 refractive index is set to the last shape's material in containers because the ray is exiting that shape
                data.n1 = containers.back()->getMaterial().refractiveIndex;
            }
        }

        bool enteredObject = false;
        int b;
        for(b = 0; b < containers.size(); b++){
            if(rayIntersects.at(a).getShape() == containers.at(b)){
                enteredObject = true;
                break;
            }
        }

        if(enteredObject){
            containers.erase(containers.begin() + b);
        }else{
            containers.push_back(rayIntersects.at(a).getShape());
        }

        if(rayIntersects.at(a).isEqual(i)){
            // Ray is not currently in any object
            if(containers.empty()){
                data.n2 = 1.0;
            }else{
                // n2 refractive index is set to the refractive index of the last shape that was entered
                data.n2 = containers.back()->getMaterial().refractiveIndex;
            }
        }
    }
}

// Approximates Fresnel Effect using Schlick's approximation
// Refer to "Reflections and Refractions in Ray Tracing" by Bram de Greve
float schlickApproximation(LightData data){
    // cosine of the angle between camera and normal vector
    float cos = dotProduct(data.camera, data.normal);

    if(data.n1 > data.n2){
        float n_ratio = data.n1/data.n2;
        float sin2_t = pow(n_ratio, 2)*(1.0 - pow(cos, 2));
        if(sin2_t > 1.0){
            return 1.0;
        }
        
        // Sets cos to cosine of theta_t using trig identity
        cos = sqrt(1.0 - sin2_t);
    }

    float r0 = pow(((data.n1 - data.n2)/(data.n1 + data.n2)), 2);
    return r0 + (1 - r0)*pow((1.0 - cos), 5);
}