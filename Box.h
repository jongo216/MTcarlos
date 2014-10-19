
#ifndef __BOX_H__
#define __BOX_H__

#include "typedefs.h"
#include "Object.h"
#include "Rectangle.h"

enum box_sides {TOP = 0, BOTTOM = 1, RIGHT = 2, LEFT = 3, FRONT = 4, BACK = 5};

class Box : public Object{
    public:
        Box(float width, float height, float depth, Pos3 centerPoint, Material mat)
        : Object(centerPoint, mat){
            sides_ = new Rectangle[6]{
                Rectangle(-X_AXIS*width,  Z_AXIS*depth,  centerPoint+Pos3( 0.f,  height/2, 0.f), material_),
                Rectangle( X_AXIS*width,  Z_AXIS*depth,  centerPoint+Pos3( 0.f, -height/2, 0.f), material_),
                Rectangle(-Z_AXIS*depth,  Y_AXIS*height, centerPoint+Pos3( width/2,  0.f,  0.f), material_),
                Rectangle( Z_AXIS*depth,  Y_AXIS*height, centerPoint+Pos3(-width/2,  0.f,  0.f), material_),
                Rectangle( X_AXIS*width,  Y_AXIS*height, centerPoint+Pos3( 0.f,  0.f,  depth/2), material_),
                Rectangle(-X_AXIS*width,  Y_AXIS*height, centerPoint+Pos3( 0.f,  0.f, -depth/2), material_)
            };
        };

        ~Box(){
            delete [] sides_;
        };

        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, float &distanceAlongRay, Direction *normal = NULL){
            bool retSides[6];
            float dist[6];
            for(unsigned i = 0; i < 6; ++i)
                retSides[i] = sides_[i].calculateIntersection(rayStart, rayDir, dist[i]);

            float length = INFINITY;
            float tmpLength;
            bool ret = false;
            for(unsigned i = 0; i < 6; ++i){
                if(retSides[i]){
                    if(dist[i] < length){
                        if(normal)
                            *normal = sides_[i].normal_;
                        length = dist[i];
                        ret = true;
                    }
                }
            }
            distanceAlongRay = length;
            return ret;
        };

    protected:
        Rectangle *sides_;
};

#endif

