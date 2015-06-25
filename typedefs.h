
#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <glm/glm.hpp>

typedef glm::vec4 Pos4;
typedef glm::vec3 Pos3;
typedef glm::vec3 Direction;
typedef glm::vec3 Color;        /*RGB*/

const glm::vec3 X_AXIS(1.f, 0.f, 0.f);
const glm::vec3 Y_AXIS(0.f, 1.f, 0.f);
const glm::vec3 Z_AXIS(0.f, 0.f, 1.f);

const glm::vec3 ORIGIN(0.f, 0.f, 0.f);

const glm::vec3 WHITE (1.f, 1.f, 1.f);
const glm::vec3 BLACK (0.f, 0.f, 0.f);
const glm::vec3 RED   (1.f, 0.f, 0.f);
const glm::vec3 GREEN (0.f, 1.f, 0.f);
const glm::vec3 BLUE  (0.f, 0.f, 1.f);

enum MATERIAL_PROPS { LAMBERTIAN, GLOSSY, TRANSPARENT, EMISSIVE };

struct Material{
    int property;
    float Ks, Kd, specularAlpha;
    Color color;
    Material(int prop, Color col){
        property = prop;
        color = col;

        if(prop == LAMBERTIAN){
            Ks = 0.0f;
            Kd = 0.8f;
            specularAlpha = 2.f;
        }
        else{
            Ks = 1.0f;
            Kd = 1.0f;
            specularAlpha = 100.f;
        }
    };
};

//config variables
#define RAY_PER_PIXEL 150
#define RAY_MAX_BOUNCE 10       //for whitted raytracing
#define NO_SHADOW_RAYS 5
#define NO_MT_CARLO_RAYS 3
#define COLOR_BLEED 1.0f

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define AIR_INDEX 1.0f
#define GLASS_INDEX 1.5f

#define ERROR_CORRECTION 0.0001f

#endif
