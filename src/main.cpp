#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/render/ShadersUtils.h"
#include "utils/Constants.h"

using namespace glm;
using namespace std;

GLuint shaderProgram;
GLuint vao, vbo, ebo;
GLuint viewLocation, projLocation;

// View matrix
glm::mat4 view;
float obsX = -Constants::WIDTH / 2.0f, obsY = -Constants::HEIGHT / 2.0f, obsZ = 300.f;
float refX = 0.0f, refY = 0.0f, refZ = -100.f;
float vX = 0.0f;

// Projection matrix
glm::mat4 projection;
float xMin = 0.0f, xMax = Constants::WIDTH, yMin = 0.0f, yMax = Constants::HEIGHT;
float dNear = 100.0f, dFar = 500.0f;

glm::vec3 observer, referencePoint, vertical;

GLFWwindow *initializeWindow() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(Constants::WIDTH, Constants::HEIGHT, "GraficaPeCalc - proiect 2", nullptr,
                                          nullptr);
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
    shaderProgram = ShadersUtils::loadShaders("../src/shaders/shader.vert", "../src/shaders/shader.frag");

    // Locations
    viewLocation = glGetUniformLocation(shaderProgram, "viewShader");
    projLocation = glGetUniformLocation(shaderProgram, "projectionShader");
}

void initializeScene() {
    GLfloat vertices[] = {
            //            Coordinates             |             Colors
            -50.0f, -50.0f, -50.0f, 0.0f, 1.0f, 0.0f,
            50.0f, -50.0f, -50.0f, 0.0f, 0.9f, 0.0f,
            50.0f, 50.0f, -50.0f, 0.0f, 0.6f, 0.0f,
            -50.0f, 50.0f, -50.0f, 0.0f, 0.2f, 0.0f,
            -50.0f, -50.0f, 50.0f, 1.0f, 0.0f, 0.0f,
            50.0f, -50.0f, 50.0f, 0.7f, 0.0f, 0.0f,
            50.0f, 50.0f, 50.0f, 0.5f, 0.0f, 0.0f,
            -50.0f, 50.0f, 50.0f, 0.1f, 0.0f, 0.0f,
    };
    GLuint indices[] = {
            1, 0, 2, 2, 0, 3,  //  Bottom
            2, 3, 6, 6, 3, 7,  // Side
            7, 3, 4, 4, 3, 0,  // Side
            4, 0, 5, 5, 0, 1,  // side
            1, 2, 5, 5, 2, 6,  // Side
            5, 6, 4, 4, 6, 7, //  Top
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // 0 = position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1); // 1 = color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
}

void render() {
    glUseProgram(shaderProgram);

    // View matrix
    observer = glm::vec3(obsX, obsY, obsZ);
    refX = obsX;
    refY = obsY;
    referencePoint = glm::vec3(refX, refY, refZ);
    vertical = glm::vec3(vX, 1.0f, 0.0f);
    view = glm::lookAt(observer, referencePoint, vertical);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    // Projection matrix
    projection = glm::ortho(xMin, xMax, yMin, yMax, dNear, dFar);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);


    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void cleanUp() {
    glDeleteProgram(shaderProgram);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

int main() {
    GLFWwindow *window = initializeWindow();
    initializeShaders();
    initializeScene();

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(Constants::SKY_COLOR.r, Constants::SKY_COLOR.g, Constants::SKY_COLOR.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

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