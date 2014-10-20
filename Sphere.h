#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "typedefs.h"
#include "Object.h"

class Sphere : public Object{
    public:
        Sphere(Pos3 centerPoint, float radius, Material mat) : Object(centerPoint, mat){
            radius_ = radius;
            radiusSquared_ = radius_*radius_;   //use some more memory to save computations
        };

        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, float &distanceAlongRay, Direction *normal = NULL){
            // d = -(l dot (o-c)) +- sqrt( (l dot (o-c))² - ||o-c|| -r²)

            // shortest distance between ray origin and center of sphere (o-c)
            Direction shortestDistance = centerPoint_ - rayStart;
            // center - start because we want vectors facing the same direction
            float cosTheta = glm::dot(rayDir, shortestDistance);
            //angle theta greater than 90 deg, ray going away from sphere

            if(cosTheta < 0.f)
                return false;

            float distCosThetaSquared = glm::dot(shortestDistance, shortestDistance) - cosTheta*cosTheta;
            if( distCosThetaSquared > radiusSquared_)
                return false;

            //choose lowest d for intersection closest to ray origin.
            float squareRoot = std::sqrt(radiusSquared_ - distCosThetaSquared);
            distanceAlongRay = cosTheta - squareRoot;

            //second intersection on the back side related to the ray origin
            if(distanceAlongRay <= 0.f + ERROR_CORRECTION)
                distanceAlongRay = cosTheta + squareRoot;

            if(normal)
                *normal = glm::normalize(rayStart + rayDir*distanceAlongRay - centerPoint_);

            return true;
        };

    protected:
        float radius_;
        float radiusSquared_;
};

#endif

