
#ifndef __RAY_H__
#define __RAY_H__

#include "typedefs.h"

class Ray{
    public:
        Ray(){
            parentRay_      = NULL;
            childRays_      = NULL;
            numChilds_      = 0;
            insideObject_   = true;
            color_          = RED;
        };
        void setStartPoint(const Point start){ startPoint_ = start; };
        void setDirection(Direction dir){ direction_ = glm::normalize(dir); };
        void setImportance(const float imp){ importance_ = imp; };

        Color computeColor(){
            return color_;
        };

    protected:
        Point           startPoint_;
        Direction       direction_;
        float           importance_;
        Color           color_;         /* RGB vec3 */
        Ray             *parentRay_;
        Ray             *childRays_;
        unsigned char   numChilds_;     /* if = 0 the ray is finalNode */
        bool            insideObject_;
};

#endif

