#ifndef GC_COLOR_H
#define GC_COLOR_H

#include <string>
#include <glm/glm.hpp>

using namespace std;

class Color {
public:
    static glm::vec3 fromHex(string hexCode);
};


#endif //GC_COLOR_H
