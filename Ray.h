
#ifndef __RAY_H__
#define __RAY_H__

#include <vector>

#include "typedefs.h"
#include "Object.h"

typedef std::pair<Pos4, Material> distMatPair;

class Ray{
    public:
        Ray();
        Ray(const Pos3 &start, const Direction &dir, float importance, bool inside, const Ray *parent);
        ~Ray();

        inline void setStartPoint(const Pos3 start){ startPoint_ = start; };
        inline void setDirection(Direction dir){ direction_ = glm::normalize(dir); };
        inline void setImportance(const float imp){ importance_ = imp; };

        // The position of the camera is used to calculate view direction
        Color computeColor(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights);

    protected:
        Color computeLocalLighting(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, const distMatPair &pointPair);
        //variables
        Pos3            startPoint_;    // starting point of ray
        Direction       direction_;
        float           importance_;
        Color           color_;         // RGB vec3
        const Ray       *parentRay_;
        Ray             *childRays_;
        unsigned char   numChilds_;     // if = 0 the ray is finalNode
        bool            insideObject_;
};

#endif

