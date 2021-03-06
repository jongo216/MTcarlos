
#ifndef __RAY_H__
#define __RAY_H__

#include <vector>
#include <random>

#include "typedefs.h"
#include "Object.h"
#include "Light.h"

typedef std::pair<Pos4, Material> distMatPair;

class Ray{
    public:
        Ray();

        inline void setStartPoint(const Pos3 &start){ startPoint_ = start; };
        inline void setDirection(const Direction &dir){ direction_ = dir; };
        inline void setImportance(const float &imp){ importance_ = imp; };
        inline void setInsideObject(const bool &inside){ insideObject_ = inside; };
        inline void setRussianP(const float &p){ russianP_ = p;};

        // The position of the camera is used to calculate view direction
        Color computeColor(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, unsigned iteration);

    protected:
        Color computeLocalLighting(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights, const distMatPair &pointPair);
        Ray   computeReflectionRay(const Direction &normal, const std::vector<Light*> &lights, const distMatPair &pointPair);
        Ray   computeRefractionRay(const Direction &normal, const distMatPair &pointPair);
        float computeBRDF(const Direction &normal, const distMatPair &pointPair, Direction &reflection);
        bool  russianRoulette(const Material &m);

        //variables
        Pos3            startPoint_;    // starting point of ray
        Direction       direction_;
        float           importance_;
        Color           color_;         // RGB vec3
        bool            insideObject_;
        float           russianP_;

        //define random number generator
        static std::default_random_engine GENERATOR_;
        static unsigned SEED_;
};

#endif

