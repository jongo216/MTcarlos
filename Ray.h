
#ifndef __RAY_H__
#define __RAY_H__

#include "typedefs.h"
#include "Object.h"

class Ray{
    public:
        Ray(){
            parentRay_      = NULL;
            childRays_      = NULL;
            numChilds_      = 0;
            insideObject_   = true;
            color_          = BLACK;
        };
        void setStartPoint(const Pos3 start){ startPoint_ = start; };
        void setDirection(Direction dir){ direction_ = glm::normalize(dir); };
        void setImportance(const float imp){ importance_ = imp; };

        Color computeColor(std::vector<Object*> *obj){
            //loop through the whole scene for each ray to determine intersection points
            Pos3 intersect;
            float distance = 0.f;
            for(unsigned i = 0; i < obj->size(); ++i){
                 if(obj->at(i)->calculateIntersection(startPoint_, direction_, intersect)){
                    //got an intersection
                    /*float tmpDist = glm::length(startPoint_ - intersect);
                    //find object intersection closest to the camera;
                    if(distance == 0 || distance > ){
                        distance = tmpDist;
                    }*/
                    return obj->at(i)->getColor();
                }
            }
            return color_;
        };

    protected:
        Pos3            startPoint_;
        Direction       direction_;
        float           importance_;
        Color           color_;         /* RGB vec3 */
        Ray             *parentRay_;
        Ray             *childRays_;
        unsigned char   numChilds_;     /* if = 0 the ray is finalNode */
        bool            insideObject_;
};

#endif

