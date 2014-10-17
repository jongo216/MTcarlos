
#ifndef __RAY_H__
#define __RAY_H__

#include <algorithm>

#include "typedefs.h"
#include "Object.h"

inline bool compareDistance(const std::pair<float, Color> &first, const std::pair<float, Color> &second){
    return first.first < second.first;
}

class Ray{
    public:
        Ray(){
            parentRay_      = NULL;
            childRays_      = NULL;
            numChilds_      = 0;
            insideObject_   = true;
            color_          = BLACK;
        };
        inline void setStartPoint(const Pos3 start){ startPoint_ = start; };
        inline void setDirection(Direction dir){ direction_ = glm::normalize(dir); };
        inline void setImportance(const float imp){ importance_ = imp; };

        Color computeColor(std::vector<Object*> *obj){
            //loop through the whole scene for each ray to determine intersection points
            Pos3 intersect;
            std::vector<std::pair<float, Color> > depthTest;
            float distance;

            for(unsigned i = 0; i < obj->size(); ++i){
                 if(obj->at(i)->calculateIntersection(startPoint_, direction_, intersect)){
                    //got an intersection, save the intersection distance to the camera
                    // and color to do depth test
                    distance = glm::length(startPoint_ - intersect);
                    //std::pair<float, Color> col(distance, obj->at(i)->getColor();
                    depthTest.push_back( std::pair<float, Color>( distance, obj->at(i)->getColor() ) );
                    //return obj->at(i)->getColor();
                }
            }
            if(!depthTest.empty()){
                std::sort(depthTest.begin(), depthTest.end(), compareDistance);
                color_ = depthTest[0].second;
            }
            return color_;
        };

    protected:
        //variables
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

