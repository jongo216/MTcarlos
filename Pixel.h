
#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "typedefs.h"
#include "Camera.h"
#include "Ray.h"


class Pixel{
    public:
        //constructor
        Pixel(){ color_ = BLACK; };
        //destructor
        ~Pixel(){ delete [] rays_; };

        void initRays(unsigned row, unsigned col, const Camera *cam){
            cam_ = cam;
            rays_ = new Ray[cam_->raysPerPixel_];

            //view plane normalised and spanned by unit length vectors
            float normalizedRow = ((float)row/cam_->width_) - 0.5 + 0.5f/cam_->width_;
            float normalizedCol = ((float)col/cam_->height_) - 0.5 + 0.5f/cam_->height_;

            for(unsigned i = 0; i < cam_->raysPerPixel_; ++i){
                rays_[i].setStartPoint(cam_->position_);
                rays_[i].setImportance(1.f/cam_->raysPerPixel_);
                rays_[i].setDirection(glm::normalize(
                                                  normalizedRow * cam_->rightDirection_
                                                + normalizedCol * cam_->upDirection_
                                                +                 cam_->viewDirection_));

            }
        };

        void shootRays(/*std::vector<Object> *obj*/){
            float averageWeight = 1.f/cam_->raysPerPixel_;
            for(unsigned i = 0; i < cam_->raysPerPixel_; ++i){
                color_ += rays_[i].computeColor(/*obj*/)*averageWeight;
            }
        };
        Color getColor(){ return color_; };

    protected:
        Ray *rays_;
        Color color_;
        const Camera *cam_;
};

#endif
