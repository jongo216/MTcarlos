
#ifndef __RAY_H__
#define __RAY_H__

#include <algorithm>

#include "typedefs.h"
#include "Object.h"

inline bool compareDistance(const std::pair<float, Color> &firstElem, const std::pair<float, Color> &secondElem){
    return firstElem.first < secondElem.first;
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

        Color computeColor(std::vector<Object*> &obj, std::vector<Light*> &lights){
            //loop through the whole scene for each ray to determine intersection points
            float distanceAlongRay;
            std::vector<std::pair<float, Color> > depthTest;

            for(unsigned i = 0; i < obj.size(); ++i){
                 if(obj[i]->calculateIntersection(startPoint_, direction_, distanceAlongRay)){
                    //got an intersection, save the intersection distance to the camera
                    // and color to do depth test

                    //store the distance as and color
                    depthTest.push_back( std::pair<float, Color>(distanceAlongRay, obj[i]->getColor() ) );
                }
            }

            if(!depthTest.empty()){
                //sort intersections, we only care about the closest intersection
                std::sort(depthTest.begin(), depthTest.end(), compareDistance);

                //calculate local lighting model by sending shadow ray towards lightstource hidden or not?
                std::pair<float, Color> closestPoint = depthTest.front();
                Pos3 intersectionPoint = startPoint_ + closestPoint.first*direction_;
                Direction directionToLightsource = lights.front()->position - intersectionPoint;
                Direction directionToLightsourceNormalized = glm::normalize(directionToLightsource);

                for(unsigned i = 0; i < obj.size(); ++i){
                    if(obj[i]->calculateIntersection(intersectionPoint, directionToLightsourceNormalized, distanceAlongRay)){
                        //determine if the intersection is between intersection point and light
                        if(distanceAlongRay < glm::length(directionToLightsource) && distanceAlongRay > 0.f + ERROR_CORRECTION)
                            return BLACK;
                    }
                }
                color_ = closestPoint.second*lights.front()->color;
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

