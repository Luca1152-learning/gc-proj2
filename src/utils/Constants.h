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

    // Matrices
    static const mat4 RESIZE_MATRIX;
    static GLuint MATRIX_LOCATION;

    // Colors
    static const vec3 SKY_COLOR;
};

#endif //GC_CONSTANTS_H