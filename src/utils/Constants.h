#ifndef GC_CONSTANTS_H
#define GC_CONSTANTS_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GL/glew.h>
#include "color/Color.h"

using namespace glm;

class Constants {
public:
    // Window
    static const int WIDTH;
    static const int HEIGHT;

    // Colors
    static const vec3 COLOR_SKY;
    static const vec3 COLOR_GRASS;
    static const vec3 COLOR_ROAD;
    static const vec3 COLOR_WALLS;
    static const vec3 COLOR_ROOF;
    static const vec3 COLOR_FRAMES;
    static const vec3 COLOR_WINDOWS;
    static const vec3 COLOR_CHIMNEY;
    static const vec3 COLOR_DOOR;
    static const vec3 COLOR_DOOR_KNOB;
    static const vec3 COLOR_TREE_LEAVES;
    static const vec3 COLOR_TREE_TRUNK;

    // Shininess
    static const float SHININESS_SKY;
    static const float SHININESS_GRASS;
    static const float SHININESS_ROAD;
};

#endif //GC_CONSTANTS_H