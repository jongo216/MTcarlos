
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "typedefs.h"
#include "Object.h"

class Rectangle : public Object{
    public:
        //constructor
        Rectangle(Direction right, Direction up, Pos3 centerPoint, Color col)
        : Object(centerPoint, col){

            width_ = glm::length(right);
            height_ = glm::length(up);

            wDir_ = glm::normalize(right);
            hDir_ = glm::normalize(up);
            normal_ = glm::cross(wDir_, hDir_);
        };

        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction rayDir, Pos3 &intersect){
            //dl dot n + (l0-p0) dot n = 0
            float dotPoint  = glm::dot(centerPoint_-rayStart, normal_);
            float dotDir    = glm::dot(rayDir, normal_);

            if(dotDir == 0.0){
                //ray is contained inside the plane
                if(dotPoint == 0.0){
                    return true;
                }
                //ray parallel to plane and will never intersect
                else
                    return false;
            }
            else{
                //ray intersect plane somwhere, check boundarys of Rectangle
                float d =  dotPoint / dotDir;
                intersect = d*rayDir + rayStart;                    //world coordinates
                Pos3 checkIntersect = intersect-centerPoint_;       //center to origin

                float rightComp = glm::dot(checkIntersect, wDir_);
                float upComp = glm::dot(checkIntersect, hDir_);
                if(std::abs(rightComp) <= width_/2 && std::abs(upComp) <= height_/2){
                    return true;
                }
            }

            return false;
        };

    protected:
        Direction normal_;
        Direction hDir_;
        Direction wDir_;
        float width_;
        float height_;
};

#endif
