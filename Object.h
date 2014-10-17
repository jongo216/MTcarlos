
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "typedefs.h"

class Object{
    public:
        Object(Pos3 center, Color col) : centerPoint_(center), color_(col){};
        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, Pos3 &intersect) = 0;

        Color getColor(){ return color_; };

    protected:
        Pos3 centerPoint_;
        Color color_;
};

#endif
