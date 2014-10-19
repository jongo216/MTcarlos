
#ifndef __BOX_H__
#define __BOX_H__

#include "typedefs.h"
#include "Object.h"
#include "Rectangle.h"

class Box : public Object{
    public:
        Box(float width, float height, float depth, Pos3 centerPoint, Material mat)
        : Object(centerPoint, mat),
        top_    (-X_AXIS*width,  Z_AXIS*depth,  centerPoint+Pos3(     0.f,  height/2,      0.f), material_),
        bottom_ ( X_AXIS*width,  Z_AXIS*depth,  centerPoint+Pos3(     0.f, -height/2,      0.f), material_),
        right_  (-Z_AXIS*depth,  Y_AXIS*height, centerPoint+Pos3( width/2,  height/2,      0.f), material_),
        left_   ( Z_AXIS*depth,  Y_AXIS*height, centerPoint+Pos3(-width/2,  height/2,      0.f), material_),
        front_  ( X_AXIS*width,  Y_AXIS*height, centerPoint+Pos3(     0.f,  height/2,  depth/2), material_),
        back_   (-X_AXIS*width,  Y_AXIS*height, centerPoint+Pos3(     0.f,  height/2, -depth/2), material_)
        {};

        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, float &distanceAlongRay){
            bool retSides[6];
            float intSect[6];
            retSides[0] = top_.calculateIntersection(rayStart, rayDir, intSect[0]);
            retSides[1] = bottom_.calculateIntersection(rayStart, rayDir, intSect[1]);
            retSides[2] = right_.calculateIntersection(rayStart, rayDir, intSect[2]);
            retSides[3] = left_.calculateIntersection(rayStart, rayDir, intSect[3]);
            retSides[4] = front_.calculateIntersection(rayStart, rayDir, intSect[4]);
            retSides[5] = back_.calculateIntersection(rayStart, rayDir, intSect[5]);

            float length = INFINITY;
            float tmpLength;
            bool ret = false;
            for(unsigned i = 0; i < 6; ++i){
                if(retSides[i]){
                    //tmpLength = glm::length(intSect[i]-rayStart);
                    if(intSect[i] < length){
                        length = intSect[i];
                        //intersect = intSect[i];
                        ret = true;
                    }
                }
            }
            distanceAlongRay = length;
            return ret;
        };

    protected:
        Rectangle top_, bottom_, right_, left_, front_, back_;
};

#endif

