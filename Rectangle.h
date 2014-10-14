
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "typedefs.h"
#include "Object.h"

class Rectangle : public Object{
    public:
        //constructor
        Rectangle
        (unsigned int width, unsigned int height, Point centerPoint, Direction normal, Color col)
        : width_(width), height_(height), Object(centerPoint, col){
            //starting point, a plane in xz-plane with center point at origin
            normal_ =  Y_AXIS;
            right_  =  X_AXIS;
            up_     = -Z_AXIS;
/*
            normal_ =  Z_AXIS;
            right_  =  X_AXIS;
            up_     =  Y_AXIS;
*/
/*            glm::mat4 transformationMat = glm::translate(glm::mat4(1.f), Direction(centerPoint));

            //find quaternion to rotate plane normal_ || normal;
            Direction dest = glm::normalize(normal);

            float cosTheta = glm::dot(normal_, dest);
            Direction rotationAxis;

            if (cosTheta < -1 + 0.001f){
                // special case when vectors in opposite directions:
                // there is no "ideal" rotation axis
                // So guess one; any will do as long as it's perpendicular to start

                //rotationAxis = glm::cross(Z_AXIS, normal_);
                //rotationAxis = glm::normalize(rotationAxis);

                //return gtx::quaternion::angleAxis(180.0f, rotationAxis);


                //switch direction of normal and done!
                normal_ = dest;
            }
            else{
                rotationAxis = glm::cross(normal_, dest);

                float s = std::sqrt( (1+cosTheta)*2 );
                float invs = 1 / s;

                glm::fquat(
                    s * 0.5f,
                    rotationAxis.x * invs,
                    rotationAxis.y * invs,
                    rotationAxis.z * invs
                );
            }*/

            //normal_ = glm::normalize(normal);

            // v0--v1
            //  | o |
            // v3--v2

        };

        virtual bool calculateIntersection(const Point &rayStart, const Direction rayDir, Point &intersect){
            //dl dot n + (l0-p0) dot n = 0
            float dotPoint  = glm::dot(Direction(centerPoint_ - rayStart), normal_);
            float dotDir    = glm::dot(rayDir, normal_);

            if(dotDir == 0.0){
                //ray is contained inside the plane
                if(dotPoint == 0.0){
                    return true;
                }
                //ray parallel to plane and will never intersect
                else
                    return false;
            }
            else{
                //ray intersect plane somwhere, check boundarys of Rectangle
                float d =  dotPoint / dotDir;
                intersect = d*Point(rayDir, 1.f) + rayStart;
                //Point checkIntersect = intersect - height_*Point(up_, 1.f) - width_*Point(right_, 1.f);
                if(std::abs(intersect[0]) <= width_/2 && std::abs(intersect[2]) <= height_/2/* && intersect[2] < 0*/){
                    return true;
                }
            }

            return false;
        };

    protected:
        Direction normal_;
        Direction up_;
        Direction right_;
        float width_;
        float height_;
};

#endif
