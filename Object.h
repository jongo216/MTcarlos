
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "typedefs.h"

class Ray;

class Object{
    public:
        Object(Point center, Color col) : centerPoint_(center), color_(col){};
       //#include "Ray.h"
        virtual Point calculateIntersection(Ray const *r){ return ORIGIN; };

    protected:
        Point centerPoint_;
        Color color_;
};

#endif
