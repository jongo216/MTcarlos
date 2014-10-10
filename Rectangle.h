
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "typedefs.h"
#include "Object.h"

class Rectangle : public Object{
    public:
        //constructor
        Rectangle
        (unsigned int width, unsigned int height, Point origin, Direction normal, Color col)
        : width_(width), height_(height), Object(origin, col){
            normal_ = glm::normalize(normal);
            //calculate vertexpoints based on width height and origin

            // v0--v1
            //  | o |
            // v3--v2

            //n(a,b,c)
            //a(x - x0) + b(y - y0) + c(z - z0) = 0
            //ax + by + cz + d = 0, d = -(ax0 + by0 + cz0)


            //n(0,1,0)
            //b(y-y0) = 0

            /*v0 = Point( normal_.x*(-width_ /2.f-orign.x),
                        normal_.y*( height_/2.f-orign.y),
                        normal_.z*( 0.f - orign.z), 1.f);

            v1 = Point( normal_.x*( width_ /2.f-orign.x),
                        normal_.y*( height_/2.f-orign.y),
                        normal_.z*( 0.f - orign.z), 1.f);

            v2 = Point( normal_.x*( width_ /2.f-orign.x),
                        normal_.y*(-height_/2.f-orign.y),
                        normal_.z*( 0.f - orign.z), 1.f);

            v3 = Point( normal_.x*(-width_ /2.f-orign.x),
                        normal_.y*(-height_/2.f-orign.y),
                        normal_.z*( 0.f - orign.z), 1.f);
                        */

        };

        virtual Point calculateIntersection(/*Ray r*/){
            return ORIGIN;
        };

    protected:
        Point v0_,v1_,v2_,v3_;
        Direction normal_;
        float width_;
        float height_;
};

#endif
