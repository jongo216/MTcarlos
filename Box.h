
#ifndef __BOX_H__
#define __BOX_H__

#include "typedefs.h"
#include "Object.h"
#include "Rectangle.h"

class Box : public Object{
    public:
        Box(float width, float height, float depth, Pos3 centerPoint, Color col)
        : Object(centerPoint, col),
        top_    (-X_AXIS*width,  Z_AXIS*depth,  centerPoint+Pos3(     0.f,  height/2,      0.f), color_),
        bottom_ ( X_AXIS*width,  Z_AXIS*depth,  centerPoint+Pos3(     0.f, -height/2,      0.f), color_),
        right_  (-Z_AXIS*depth,  Y_AXIS*height, centerPoint+Pos3( width/2,  height/2,      0.f), color_),
        left_   ( Z_AXIS*depth,  Y_AXIS*height, centerPoint+Pos3(-width/2,  height/2,      0.f), color_),
        front_  ( X_AXIS*width,  Y_AXIS*height, centerPoint+Pos3(     0.f,  height/2,  depth/2), color_),
        back_   (-X_AXIS*width,  Y_AXIS*height, centerPoint+Pos3(     0.f,  height/2, -depth/2), color_)
        {};

        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, Pos3 &intersect){
            bool retSides[6];
            Pos3 intSect[6];
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
                    tmpLength = glm::length(intSect[i]-rayStart);
                    if(tmpLength < length){
                        length = tmpLength;
                        intersect = intSect[i];
                        ret = true;
                    }
                }
            }
            return ret;
        };

    protected:
        Rectangle top_, bottom_, right_, left_, front_, back_;
};

#endif

