
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include <stdlib.h>

#include "typedefs.h"
#include "Object.h"

class Rectangle : public Object{
    friend class Box;
    public:
        //constructor
        Rectangle(Direction right, Direction up, Pos3 centerPoint, Material mat)
        : Object(centerPoint, mat){

            width_ = glm::length(right);
            height_ = glm::length(up);

            wDir_ = glm::normalize(right);
            hDir_ = glm::normalize(up);
            normal_ = glm::cross(wDir_, hDir_);
        };

        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, float &distanceAlongRay, Direction *normal = NULL){
            //dl dot n + (l0-p0) dot n = 0
            float dotPoint  = glm::dot(centerPoint_-rayStart, normal_);
            float dotDir    = glm::dot(rayDir, normal_);

            if(dotDir == 0.0){
                //ray is contained inside the plane
                if(dotPoint == 0.0)
                    return false;
                //ray parallel to plane and will never intersect
                else
                    return false;
            }
            else{
                //ray intersect plane somwhere, check boundarys of Rectangle
                distanceAlongRay = dotPoint / dotDir;

                if(distanceAlongRay > 0.f + ERROR_CORRECTION){
                    Pos3 checkIntersect = (distanceAlongRay*rayDir + rayStart)-centerPoint_;       //center world coordinates to origin

                    // use vector projection to determine if intersection is inside rectangle
                    float rightComp = glm::dot(checkIntersect, wDir_);
                    float upComp = glm::dot(checkIntersect, hDir_);
                    if(std::abs(rightComp) <= width_/2 && std::abs(upComp) <= height_/2){
                        if(normal)
                            *normal = normal_;
                        return true;
                    }
                }
            }
            return false;
        };

        Pos3 getRandomPoint(){
            float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f;   //random between 0 and 1
            float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f;   //shift -0.5 to compensate for origin in center

            return Pos3(rx*wDir_*(width_/2) + ry*hDir_*(height_/2) + centerPoint_);
        };

        inline Direction getNormal(){ return normal_; };

    protected:
        Direction normal_;
        Direction hDir_;
        Direction wDir_;
        float width_;
        float height_;
};

#endif
