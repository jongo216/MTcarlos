
#ifndef __PIXEL_H__
#define __PIXEL_H__

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
            float normalizedRow = ((float)row/cam->width_) - 0.5 + 0.5f/cam->width_;
            float normalizedCol = ((float)col/cam->height_) - 0.5 + 0.5f/cam->height_;

            for(unsigned i = 0; i < RAY_PER_PIXEL; ++i){
                rays_[i].setStartPoint(cam->position_);
                rays_[i].setImportance(1.f/RAY_PER_PIXEL);
                rays_[i].setDirection(glm::normalize(
                                                  normalizedRow * cam->rightDirection_
                                                + normalizedCol *-cam->upDirection_     // minus notation due to center point transformation in image plane otherwise image is upside down
                                                +                 cam->viewDirection_));
            }
        };

        void shootRays(std::vector<Object*> *obj){
            float averageWeight = 1.f/RAY_PER_PIXEL;
            for(unsigned i = 0; i < RAY_PER_PIXEL; ++i){
                color_ += rays_[i].computeColor(obj)*averageWeight;
            }
        };

        Color getColor(){ return color_; };

    protected:
        Ray *rays_;
        Color color_;
};

#endif
