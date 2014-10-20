
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "typedefs.h"

class Object{
    public:
        Object(Pos3 center, Material mat) : centerPoint_(center), material_(mat){};
        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, float &distanceAlongRay, Direction *normal = NULL) = 0;

        inline Material getMaterial(){ return material_; };

    protected:
        Pos3 centerPoint_;
        Material material_;
};

#endif
