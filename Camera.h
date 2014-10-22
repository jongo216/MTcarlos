
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <string>
#include <vector>

#include "typedefs.h"
#include "Object.h"
#include "Light.h"

//Pre-define class, include in cpp
class Pixel;

class Camera{
    friend class Pixel;
    public:
        //constructor
        Camera(const unsigned width, const unsigned height, Pos3 pos, const float dist, Direction viewDir, Direction upDir);

        //destructor
        ~Camera();

        void writePPM(const std::string fileName, const std::vector<Object*> &obj, const std::vector<Light*> &lights);

    protected:
        const unsigned width_;
        const unsigned height_;
        Pos3 position_;
        Direction viewDirection_;
        Direction upDirection_;
        Direction rightDirection_;

        float distanceToImagePlane_;

        Pixel *pixels_;
};

#endif
