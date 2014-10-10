#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "typedefs.h"
#include "Object.h"

class Sphere : Object{
    public:
        Sphere();
        virtual Point CalculateIntersection();

    protected:
        vec3 centerPoint_;
        float radius_;
}

#endif

