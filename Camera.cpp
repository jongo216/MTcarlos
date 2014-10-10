
#include <fstream>
#include <iostream>
#include <iomanip>

#include "Camera.h"

Camera::Camera(const unsigned width, const unsigned height, unsigned rpp, Point pos, const float dist, Direction viewDir, Direction upDir)
            : width_(width), height_(height), raysPerPixel_(rpp), position_(pos), distanceToImagePlane_(dist){

    viewDirection_ = glm::normalize(viewDir);
    upDirection_ = glm::normalize(upDir);

    rightDirection_ = glm::cross(viewDirection_,upDirection_);
    upDirection_ = glm::cross(rightDirection_, viewDirection_);

    pixels_ = new Pixel[width_*height_];
    //init pixels
    for(unsigned row = 0; row < width_; ++row){
        for(unsigned col = 0; col < height_; ++col){
            pixels_[row*col].initRays(row, col, this);
        }
    }
}

void Camera::writePPM(const std::string fileName/*, std::vector<Object> *obj*/){
    //do the raytracing
    int progress;
    for(unsigned i = 0; i < width_*height_; ++i){
        pixels_[i].shootRays(/*obj*/);
        //show progress in console
        progress = (float)i/(width_*height_-1)*100;
        if(progress % 5 == 0){
            std::cout << "\r" << progress << "% completed.";
            std::cout.flush();
        }
    }

    Color color;
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs((fileName + ".ppm").c_str(), std::ofstream::out | std::ofstream::binary);
    //ppm header
    ofs << "P6\n" << width_ << " " << height_ << "\n255\n";
    //ppm data
    for (unsigned i = 0; i < width_*height_; ++i) {
        color = pixels_[i].getColor();
        ofs << (unsigned char)(std::min(1.f, color.r) * 255)
            << (unsigned char)(std::min(1.f, color.g) * 255)
            << (unsigned char)(std::min(1.f, color.b) * 255);
    }
    ofs.close();
}
