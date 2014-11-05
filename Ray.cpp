
#include <algorithm>
#include <limits>
#include <chrono>

#include "Ray.h"

// function to sort based on distance
inline bool compareDistance(const distMatPair &firstElem, const distMatPair &secondElem){
    return firstElem.first[3] < secondElem.first[3];
};

//initialize static random number generator;
unsigned Ray::SEED_ = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine Ray::GENERATOR_(Ray::SEED_);

//default Constructor, called when initializing array
Ray::Ray(){
    insideObject_   = false;
    color_          = BLACK;
    russianP_       = 1.f;
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
        distMatPair closestIntersection = depthTest.front();

        if(russianRoulette(closestIntersection.second) ){ /* keep this for whitted ray tracing */ // iteration <= RAY_MAX_BOUNCE){

            surfaceNormal = Direction(closestIntersection.first);

            switch(closestIntersection.second.property){
                case LAMBERTIAN:
                {
                    float childImportance = 0.f;
                    //if(NO_MT_CARLO_RAYS != 0){
                    for(unsigned i = 0; i < NO_MT_CARLO_RAYS; ++i){
                        Ray reflectedRay = computeReflectionRay(surfaceNormal, lights, closestIntersection);
                        reflectedRay.setImportance(reflectedRay.importance_*COLOR_BLEED/NO_MT_CARLO_RAYS);
                        childImportance += reflectedRay.importance_;
                        if(childImportance > 0.f)
                            reflectedRayColor += reflectedRay.computeColor(camPos, obj, lights, ++iteration);
                    }
                    localLighting = (1.f-childImportance)*computeLocalLighting(camPos, obj, lights, closestIntersection);
                    break;
                }

                case GLOSSY:
                {
                    Ray reflectedRay = computeReflectionRay(surfaceNormal, lights, closestIntersection);
                    reflectedRayColor = reflectedRay.computeColor(camPos, obj, lights, ++iteration);
                    break;
                }

                case TRANSPARENT:
                {
                    Ray refractedRay;
                    bool refraction = false;
                    if(insideObject_){
                        //alpha testing, if ray can be transmitted to air again
                        float alpha = std::acos(glm::dot(surfaceNormal, direction_));       //want positive angle
                        surfaceNormal = -surfaceNormal;
                        float alphaMax = std::asin(AIR_INDEX/GLASS_INDEX);

                        if(alpha < alphaMax){
                            refractedRay = computeRefractionRay(surfaceNormal, closestIntersection);
                            refraction = true;
                        }
                    }
                    else{
                        refractedRay = computeRefractionRay(surfaceNormal, closestIntersection);
                        refraction = true;
                    }
                    Ray reflectedRay = computeReflectionRay(surfaceNormal, lights, closestIntersection);
                    //if frefraction ray, compensate importance
                    if(refraction){
                        reflectedRay.setImportance(1.f - refractedRay.importance_);
                        refractedRayColor = refractedRay.computeColor(camPos, obj, lights, ++iteration);
                    }

                    reflectedRayColor = reflectedRay.computeColor(camPos, obj, lights, ++iteration);
                    break;
                }

                case EMISSIVE:
                {
                    localLighting = lights.front()->getColor();
                    break;
                }
            }
        } //else localLighting = computeLocalLighting(camPos, obj, lights, closestIntersection);
    }

    return importance_*(localLighting + reflectedRayColor + refractedRayColor)/russianP_;
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
            float intensity = pointPair.second.Kd * std::max(0.f, glm::dot(normal, dirToLightNormalized))           // diffuse component
                            + pointPair.second.Ks * std::pow( glm::dot(normal, halfwayVector), pointPair.second.specularAlpha);     // specular component
            returnColor += intensity*pointPair.second.color*lights.front()->getColor();
        }
    }
    return returnColor/(float)numberOfShadowRays;
};

Ray Ray::computeReflectionRay(const Direction &normal, const std::vector<Light*> &lights, const distMatPair &pointPair){
    Pos3        reflectedRayPos = startPoint_+(pointPair.first[3]+ERROR_CORRECTION) * direction_;
    Direction   perfectReflection = direction_ - 2.f*glm::dot(direction_, normal)*normal;

    Ray ret;
    ret.setStartPoint(reflectedRayPos);
    ret.setImportance( computeBRDF(normal, pointPair, perfectReflection) );
    ret.setDirection(glm::normalize(perfectReflection));
    ret.setInsideObject(insideObject_);
    ret.setRussianP(russianP_);

    return ret;
};


Ray Ray::computeRefractionRay(const Direction &normal, const distMatPair &pointPair){
    float n1 = insideObject_ ? GLASS_INDEX : AIR_INDEX;
    float n2 = insideObject_ ? AIR_INDEX : GLASS_INDEX;
    float refractiveIndex = n1/n2;

    float NdotDir = glm::dot(normal, direction_);

    Pos3 refractedRayPos = startPoint_ + (pointPair.first[3]+ERROR_CORRECTION)*direction_;

    Direction refractedRayDir = glm::normalize(refractiveIndex*direction_ + normal*( -refractiveIndex*NdotDir - std::sqrt(1.f - (refractiveIndex*refractiveIndex) * (1.f-NdotDir*NdotDir)) ));

    float refractedRayImportance = (4.f*n1*n2) / ((n1+n2)*(n1+n2)); //approximate computation

    Ray ret;
    ret.setStartPoint(refractedRayPos);
    ret.setDirection(refractedRayDir);
    ret.setImportance(refractedRayImportance);
    ret.setInsideObject(!insideObject_);
    ret.setRussianP(russianP_);

    return ret;
};

float Ray::computeBRDF(const Direction &normal, const distMatPair &pointPair, Direction &reflection){
    if(pointPair.second.property == LAMBERTIAN){
        Direction perfectReflection = reflection;
        //random numbers for azimuth and inclination angle

        //inclination θ, azimuth φ
        // u1 θ, u2 φ
        double u1 = std::generate_canonical<double, std::numeric_limits<double>::digits>(GENERATOR_);   //generate in the interval[0,1)
        double u2 = std::generate_canonical<double, std::numeric_limits<double>::digits>(GENERATOR_);   //generate in the interval[0,1)

        //psi with respect to Z axis
        Direction zRefl(std::sqrt(1.f-u1)*std::cos(2*M_PI*u2),
                        std::sqrt(1.f-u1)*std::sin(2*M_PI*u2),
                        std::sqrt(1.f-u1)
                       );

        //rotate accordingly to normal
        Direction t = normal;

        unsigned pos = 0;
        float m = t[pos]*t[pos];
        for(unsigned j = 1; j < 3; ++j){
            if(m > t[j]*t[j]){
                m = t[j]*t[j];
                pos = j;
            }
        }
        t[pos] = 1.f;

        Direction u = glm::normalize(glm::cross(t, normal));
        Direction v =                glm::cross(normal, u);

        glm::mat3 rot(u, v, normal);
        reflection = (rot*zRefl);

        return std::min(1.f, (pointPair.second.specularAlpha+2.f)/(2.f*NO_MT_CARLO_RAYS) * std::pow(glm::dot(perfectReflection, reflection), pointPair.second.specularAlpha));
    }
    return 1.f;
};

bool Ray::russianRoulette(const Material &m){
    float randomDraw = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float alpha = 0.8f/m.specularAlpha;     //absorption probability
    russianP_ *= (1.f-alpha);

    if(randomDraw < russianP_)
        return true;

    return false;
};
