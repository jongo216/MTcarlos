
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "typedefs.h"

class Object{
    public:
        Object(Point center, Color col) : centerPoint_(center), color_(col){};
        virtual bool calculateIntersection(const Point &rayStart, const Direction rayDir, Point &intersect) = 0;

        Color getColor(){ return color_; };

    protected:
        Point centerPoint_;
        Color color_;
};

#endif
