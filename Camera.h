
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <string>

#include "typedefs.h"

class Camera{
    friend class Pixel;
    public:
        //constructor
        Camera(const unsigned width, const unsigned height, Point pos, const float dist, Direction viewDir, Direction upDir);

        //destructor
        ~Camera(){ delete [] pixels_; };

        void writePPM(const std::string fileName);

    protected:
        const unsigned width_;
        const unsigned height_;
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
