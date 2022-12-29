#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "utils/render/ShadersUtils.h"
#include "utils/Constants.h"

using namespace glm;
using namespace std;

GLuint shaderProgramId;

GLFWwindow *initializeWindow() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(Constants::WIDTH, Constants::HEIGHT, "macOS OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);

    return window;
}

void initializeShaders() {
    shaderProgramId = ShadersUtils::loadShaders("../src/shaders/shader.vert", "../src/shaders/shader.frag");
    Constants::MATRIX_LOCATION = glGetUniformLocation(shaderProgramId, "matrix");
}

void render() {
    glUseProgram(shaderProgramId);
}

void cleanUp() {
    glDeleteProgram(shaderProgramId);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

int main() {
    GLFWwindow *window = initializeWindow();
    initializeShaders();

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(Constants::SKY_COLOR.r, Constants::SKY_COLOR.g, Constants::SKY_COLOR.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUp();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}