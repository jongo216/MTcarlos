
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <string>
#include <vector>

#include "typedefs.h"
#include "Object.h"

//Pre-define class, include in cpp
class Pixel;

class Camera{
    friend class Pixel;
    public:
        //constructor
        Camera(const unsigned width, const unsigned height, Point pos, const float dist, Direction viewDir, Direction upDir);

        //destructor
        ~Camera();

        void writePPM(const std::string fileName, std::vector<Object*> *obj);

    protected:
        const unsigned width_;
        const unsigned height_;
        Point position_;
        Direction viewDirection_;
        Direction upDirection_;
        Direction rightDirection_;

        float distanceToImagePlane_;

        Pixel *pixels_;
};

#endif
