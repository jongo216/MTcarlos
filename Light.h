
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "typedefs.h"
#include "Rectangle.h"

class Light{
    public:
        Light(const Pos3 &pos, const Color &col, bool point = true, float width = 1.f, float depth = 1.f)                 //lambertian emitter
        : position_(pos), pointLight_(point), area_(X_AXIS*width,  Z_AXIS*depth,  position_, Material(EMISSIVE, col)){};

        inline Color getColor(){ return area_.getMaterial().color; };
        inline Rectangle getArea(){ return area_; };
        inline Pos3 getPosition(){ return position_; };
        inline Pos3 getRandomPosition(){
            if(!pointLight_)
                return area_.getRandomPoint() + area_.getNormal()* 50.f;
            else
                return position_ + area_.getNormal()* 50.f;
        };
        inline bool isPointLight(){ return pointLight_; };

    protected:
        Pos3 position_;
        bool pointLight_;   //point light or area light?

        Rectangle area_;    //if area light
};

#endif
