
#include <algorithm>

#include "Ray.h"

// function to sort based on distance
inline bool compareDistance(const distMatPair &firstElem, const distMatPair &secondElem){
    return firstElem.first[3] < secondElem.first[3];
};

//default Constructor, called when initializing array
Ray::Ray(){
    parentRay_      = NULL;
    childRays_      = NULL;
    numChilds_      = 0;
    insideObject_   = false;
    color_          = BLACK;
};

//destructor
Ray::~Ray(){
    delete [] childRays_;
};

Color Ray::computeColor(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, unsigned iteration){
    // Loop through the scene for each ray to determine intersection points
    float distanceAlongRay;
    std::vector<distMatPair> depthTest;
    Direction surfaceNormal;

    //store the distance and surface normal from ray origin to intersection and color
    for(unsigned i = 0; i < obj.size(); ++i)
         if(obj[i]->calculateIntersection(startPoint_, direction_, distanceAlongRay, &surfaceNormal))
            depthTest.push_back( distMatPair( Pos4(surfaceNormal, distanceAlongRay ), obj[i]->getMaterial()) );


    Color localLighting = BLACK;
    Color reflectedRayColor = BLACK;
    Color refractedRayColor = BLACK;

    if(!depthTest.empty()){
        // sort intersections, we only care about the closest intersection
        std::sort(depthTest.begin(), depthTest.end(), compareDistance);
        // local lighting contribution
        distMatPair closestIntersection = depthTest.front();

        //launch reflected ray
        if(iteration <= RAY_MAX_BOUNCE){ // && closestIntersection.second.property != LAMBERTIAN){

            surfaceNormal = Direction(closestIntersection.first);
            Direction directionToLightsource;

            switch(closestIntersection.second.property){
                case LAMBERTIAN:
                    localLighting = computeLocalLighting(camPos, obj, lights, closestIntersection);
                    break;

                case GLOSSY:
                    childRays_ = new Ray[1];
                    ++numChilds_;
                    //initialize reflection ray
                    computeReflectionRay(surfaceNormal, lights, closestIntersection);
                    reflectedRayColor = /*childRays_[0].importance_**/childRays_[0].computeColor(camPos, obj, lights, ++iteration);
                    //localLighting = computeLocalLighting(camPos, obj, lights, closestIntersection);
                    break;

                case TRANSPARENT:
                    if(insideObject_){
                        //alpha testing, if ray can be transmitted to air again
                        float alpha = std::acos(glm::dot(surfaceNormal, direction_));       //want positive angle
                        surfaceNormal = -surfaceNormal;
                        float alphaMax = std::asin(AIR_INDEX/GLASS_INDEX);

                        if(alpha < alphaMax){
                            childRays_ = new Ray[2];
                            ++numChilds_;
                            //initialize refraction ray
                            computeRefractionRay(surfaceNormal, closestIntersection);
                        } else
                            childRays_ = new Ray[1];
                    }
                    else{
                        childRays_ = new Ray[2];
                        ++numChilds_;
                        computeRefractionRay(surfaceNormal, closestIntersection);
                    }
                    ++numChilds_;
                    //initialize reflection ray
                    computeReflectionRay(surfaceNormal, lights, closestIntersection);
                    //if frefraction ray, compensate importance
                    if(numChilds_ == 2){
                        childRays_[0].setImportance(1.f - childRays_[1].importance_);
                        refractedRayColor = childRays_[1].computeColor(camPos, obj, lights, ++iteration);
                    }
if(!insideObject_)
                    reflectedRayColor = childRays_[0].computeColor(camPos, obj, lights, ++iteration);
else
                    reflectedRayColor = childRays_[0].computeColor(camPos, obj, lights, ++iteration);
                    break;

                case EMISSIVE:
                    localLighting = lights.front()->getColor();
                    break;
            }
        }
    }

    //check if reflected ray is carying any color, otherwise discard the ray (only local lighting model)
    float dImportance = 1.f;
    /*if(glm::dot(reflectedRayColor,reflectedRayColor) > ERROR_CORRECTION*ERROR_CORRECTION && numChilds_ > 0) //use dot instead of length to avoid using sqrt
        dImportance = 1.f - childRays_[0].importance_;

    if(glm::dot(refractedRayColor,refractedRayColor) > ERROR_CORRECTION*ERROR_CORRECTION && numChilds_ > 1) //use dot instead of length to avoid using sqrt
        dImportance -= childRays_[1].importance_;*/

    //if(glm::dot(refractedRayColor,refractedRayColor) > ERROR_CORRECTION*ERROR_CORRECTION) //use dot instead of length to avoid using sqrt
      //  dImportance -= refractedRayImportance;
    //if(refractedRayImportance > 0.f)
        //dImportance = 0.f;
    return importance_*(localLighting + reflectedRayColor + refractedRayColor);
};

Color Ray::computeLocalLighting(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, const distMatPair &pointPair){
    float distanceAlongShadowRay;

    //calculate local lighting model by sending shadow ray towards lightstource hidden or not?
    Pos3 intersectionPoint = startPoint_ + (pointPair.first[3] - ERROR_CORRECTION)*direction_;
    //needed for Blinn-Phong shading
    Direction normal(pointPair.first);  // surface normal
    Direction viewDirection = glm::normalize(camPos - intersectionPoint); // direction towards camera
    Color returnColor = BLACK;
    //determine if the light source is occluded by other objects
    unsigned numberOfShadowRays = lights.front()->isPointLight() ? 1 : NO_SHADOW_RAYS;
    for(unsigned shadowRays = 0; shadowRays < numberOfShadowRays; ++shadowRays){

        Direction dirToLight = lights.front()->getRandomPosition() - intersectionPoint;
        Direction dirToLightNormalized = glm::normalize(dirToLight);
        float lengthToLight = glm::length(dirToLight);

        bool discard = false;
        for(unsigned i = 0; i < obj.size(); ++i)
            if( obj[i]->calculateIntersection(intersectionPoint, dirToLightNormalized, distanceAlongShadowRay) )
                if(distanceAlongShadowRay < lengthToLight && distanceAlongShadowRay > 0.f + ERROR_CORRECTION){ //shadow rays pass through transparant objects
                    if(obj[i]->getMaterial().property == TRANSPARENT){
                        returnColor += pointPair.second.color*lights.front()->getColor()*0.3f;
                        discard = true;
                        break;
                    } else{
                        discard = true;
                        break;
                    }
                }
        // Blinn-Phong shading
        if(!discard){
            Direction halfwayVector = glm::normalize(dirToLightNormalized + viewDirection);
            float intensity = pointPair.second.Kd * std::max(0.f, glm::dot(normal, dirToLightNormalized))   // diffuse component
                            + pointPair.second.Ks * std::pow( glm::dot(normal, halfwayVector), ALPHA);       // specular component
            returnColor += intensity*pointPair.second.color*lights.front()->getColor();
        }
    }
    return returnColor/(float)numberOfShadowRays;
};

void Ray::computeReflectionRay(const Direction &normal, const std::vector<Light*> &lights, const distMatPair &pointPair){
    Pos3        reflectedRayPos = startPoint_+(pointPair.first[3]+ERROR_CORRECTION) * direction_;
    Direction   reflectedRayDir = glm::normalize(direction_ - 2.f*glm::dot(direction_, normal)*normal);
//#include<stdio.h>
//    if(insideObject_)
//    printf("%f %f\n", glm::length(reflectedRayPos - Pos3(0.f, 100.f, 0.f)), pointPair.first[3]);

    //Direction   dirToLight      = glm::normalize(lights.front()->getPosition() - reflectedRayPos);

    float BRDF = 1.f;//pointPair.second.Ks * std::pow( glm::dot(dirToLight, reflectedRayDir ), ALPHA) + pointPair.second.Kd;
    float reflectedRayImportance = BRDF;//*glm::dot(-direction_, normal);//importance_*glm::dot(-direction_, normal);

    childRays_[0].setStartPoint(reflectedRayPos);
    childRays_[0].setDirection(reflectedRayDir);
    childRays_[0].setImportance(reflectedRayImportance);
    childRays_[0].setInsideObject(insideObject_);
    childRays_[0].setParent(this);
};


void Ray::computeRefractionRay(const Direction &normal, const distMatPair &pointPair){
    float n1 = insideObject_ ? GLASS_INDEX : AIR_INDEX;
    float n2 = insideObject_ ? AIR_INDEX : GLASS_INDEX;
    float refractiveIndex = n1/n2;

    float NdotDir = glm::dot(normal, direction_);

    Pos3 refractedRayPos = startPoint_ + (pointPair.first[3]+ERROR_CORRECTION)*direction_;

    Direction refractedRayDir = glm::normalize(refractiveIndex*direction_ + normal*( -refractiveIndex*NdotDir - std::sqrt(1.f - (refractiveIndex*refractiveIndex) * (1.f-NdotDir*NdotDir)) ));

    float refractedRayImportance = (4.f*n1*n2) / ((n1+n2)*(n1+n2)); //approximate computation

    childRays_[1].setStartPoint(refractedRayPos);
    childRays_[1].setDirection(refractedRayDir);
    childRays_[1].setImportance(refractedRayImportance);
    childRays_[1].setInsideObject(!insideObject_);
    childRays_[1].setParent(this);
};
