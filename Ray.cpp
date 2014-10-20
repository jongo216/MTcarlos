
#include <algorithm>

#include "Ray.h"

// function to sort based on distance
inline bool compareDistance(const distMatPair &firstElem, const distMatPair &secondElem){
    return firstElem.first[3] < secondElem.first[3];
};

Color Ray::computeColor(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights){
    // Loop through the scene for each ray to determine intersection points
    float distanceAlongRay;
    std::vector<distMatPair> depthTest;
    Direction surfaceNormal;

    //store the distance and surface normal from ray origin to intersection and color
    for(unsigned i = 0; i < obj.size(); ++i)
         if(obj[i]->calculateIntersection(startPoint_, direction_, distanceAlongRay, &surfaceNormal))
            depthTest.push_back( distMatPair( Pos4(surfaceNormal, distanceAlongRay ), obj[i]->getMaterial()) );


    Color localLighting = BLACK;
    if(!depthTest.empty()){
        // sort intersections, we only care about the closest intersection
        std::sort(depthTest.begin(), depthTest.end(), compareDistance);
        // local lighting contribution
        localLighting = computeLocalLighting(camPos, obj, lights, depthTest.front());
    }
    return localLighting;
};

Color Ray::computeLocalLighting(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, const distMatPair &pointPair){
    float distanceAlongShadowRay;

    //calculate local lighting model by sending shadow ray towards lightstource hidden or not?
    Pos3 intersectionPoint = startPoint_ + pointPair.first[3]*direction_;
    Direction directionToLightsource = lights.front()->position - intersectionPoint;
    Direction directionToLightsourceNormalized = glm::normalize(directionToLightsource);
    float lengthToLightsource = glm::length(directionToLightsource);

    //determine if the light source is occluded by other objects
    for(unsigned i = 0; i < obj.size(); ++i)
        if( obj[i]->calculateIntersection(intersectionPoint, directionToLightsourceNormalized, distanceAlongShadowRay) )
            if(distanceAlongShadowRay < lengthToLightsource && distanceAlongShadowRay > 0.f + ERROR_CORRECTION)// && obj[i]->getMaterial().property != TRANSPARENT) //shadow rays pass through transparant objects
                return BLACK;

    //phong shading
    Direction normal(pointPair.first);  // surface normal
    //directionToLightsourceNormalized; // direction towards lightsource
    Direction viewDirection = glm::normalize(camPos - intersectionPoint); // direction towards camera
    Direction halfwayVector = glm::normalize(directionToLightsourceNormalized + viewDirection);

    float intensity = std::max(0.f, glm::dot(normal, directionToLightsourceNormalized)); // diffuse component
    if(pointPair.second.property != LAMBERTIAN)
        intensity += std::pow( glm::dot(normal, halfwayVector), 100); // specular component

    return intensity*pointPair.second.color*lights.front()->color;
};
