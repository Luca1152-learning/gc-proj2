#include "Color.h"

glm::vec3 Color::fromHex(string hexCode) {
    int r, g, b;
    sscanf(hexCode.c_str(), "#%02x%02x%02x", &r, &g, &b);

    return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
}