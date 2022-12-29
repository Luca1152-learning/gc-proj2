#ifndef GC_SHADERSUTILS_H
#define GC_SHADERSUTILS_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class ShadersUtils {
public:
    static GLuint loadShaders(const char *vertexShaderPath, const char *fragShaderPath);
};

#endif //GC_SHADERSUTILS_H
