
#ifndef __BOX_H__
#define __BOX_H__

#include "typedefs.h"
#include "Rectangle.h"

class Box : Rectangle{
    public:
        Box();
        virtual Point calculateIntersection();

    protected:
        Rectangle top_, bottom_, right_, left_, front_, back_;
        float depth_;
}

#endif

