#include "Constants.h"

const int Constants::WIDTH = 1280;
const int Constants::HEIGHT = 720;

const mat4 Constants::RESIZE_MATRIX = ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT));
GLuint Constants::MATRIX_LOCATION = 0;

const vec3 Constants::SKY_COLOR = Color::fromHex("#9DD5DD");