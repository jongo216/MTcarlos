
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

const glm::vec3 WHITE   (1.f, 1.f, 1.f);
const glm::vec3 BLACK   (0.f, 0.f, 0.f);
const glm::vec3 RED     (1.f, 0.f, 0.f);
const glm::vec3 GREEN   (0.f, 1.f, 0.f);
const glm::vec3 BLUE    (0.f, 0.f, 1.f);

const float ERROR_CORRECTION = 0.0001f;

struct Material{
    bool diffuse;
    Color color;
    Material(bool diff, Color col){ diffuse = diff; color = col; };
};

struct Light{
    Pos3 position;
    //pointlight
    Color color;
};

//config variables
#define RAY_PER_PIXEL 1
#define RAY_MAX_BOUNCE 1

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#endif
