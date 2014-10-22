
#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <stdlib.h>

#include "typedefs.h"
#include "Camera.h"
#include "Ray.h"


class Pixel{
    public:
        //constructor
        Pixel(){ rays_ = new Ray[RAY_PER_PIXEL]; color_ = BLACK; };
        //destructor
        ~Pixel(){ delete [] rays_; };

        void initRays(unsigned row, unsigned col, const Camera *cam){
            //view plane normalised and spanned by unit length vectors
            for(unsigned i = 0; i < RAY_PER_PIXEL; ++i){

                float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);   //random between 0 and 1
                float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

                float normalizedRow = ((float)row/cam->width_) - 0.5 + rx/cam->width_;
                float normalizedCol = ((float)col/cam->height_) - 0.5 + ry/cam->height_;

                rays_[i].setStartPoint(cam->position_);
                rays_[i].setImportance(1.f/RAY_PER_PIXEL);
                rays_[i].setDirection(glm::normalize(
                                                  normalizedRow * cam->rightDirection_*((float)cam->width_/cam->height_) //to get right proporsions
                                                + normalizedCol *-cam->upDirection_     // minus notation due to center point transformation in image plane otherwise image is upside down
                                                +                 cam->viewDirection_));
            }
        };

        void shootRays(const Pos3 &camPos, const std::vector<Object*> &obj, const std::vector<Light*> &lights){
            for(unsigned i = 0; i < RAY_PER_PIXEL; ++i){
                color_ += rays_[i].computeColor(camPos, obj, lights, 0);
            }
        };

        inline Color getColor(){ return color_; };

    protected:
        Ray *rays_;
        Color color_;
};

#endif
