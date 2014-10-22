
#ifndef __RAY_H__
#define __RAY_H__

#include <vector>

#include "typedefs.h"
#include "Object.h"
#include "Light.h"

typedef std::pair<Pos4, Material> distMatPair;

class Ray{
    public:
        Ray();
        ~Ray();

        inline void setStartPoint(const Pos3 &start){ startPoint_ = start; };
        inline void setDirection(const Direction &dir){ direction_ = glm::normalize(dir); };
        inline void setImportance(const float &imp){ importance_ = imp; };
        inline void setInsideObject(const bool &inside){ insideObject_ = inside; };
        inline void setParent(const Ray *parent){ parentRay_ = parent; };

        // The position of the camera is used to calculate view direction
        Color computeColor(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, unsigned iteration);

    protected:
        Color computeLocalLighting(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, const distMatPair &pointPair);
        void computeReflectionRay(const Direction &normal, const std::vector<Light*> &lights, const distMatPair &pointPair);
        void computeRefractionRay(const Direction &normal, const distMatPair &pointPair);

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

