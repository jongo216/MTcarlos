
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <string>
#include <vector>

#include "typedefs.h"

class Camera{
    friend class Pixel;
    public:
        //constructor
        Camera(const unsigned width, const unsigned height, unsigned rpp, Point pos, const float dist, Direction viewDir, Direction upDir);

        //destructor
        ~Camera(){ delete [] pixels_; };

        //#include "Object.h"
        void writePPM(const std::string fileName/*, std::vector<Object> *obj*/);

    protected:
        const unsigned width_;
        const unsigned height_;
        unsigned raysPerPixel_;
        Point position_;
        Direction viewDirection_;
        Direction upDirection_;
        Direction rightDirection_;

        float distanceToImagePlane_;

        //include here to avoid problems
        #include "Pixel.h"
        Pixel *pixels_;
};

#endif
