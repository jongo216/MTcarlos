
#ifndef __BOX_H__
#define __BOX_H__

#include "typedefs.h"
#include "Rectangle.h"

class Box : Rectangle{
    public:
        Box();
        virtual bool calculateIntersection(const Pos3 &rayStart, const Direction &rayDir, Pos3 &intersect);

    protected:
        Rectangle top_, bottom_, right_, left_, front_, back_;
        float depth_;
}

#endif

