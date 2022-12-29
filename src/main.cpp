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

// Camera
const float CAMERA_FOV = 75.0f;
const float CAMERA_NEAR_PLANE = 0.1f;
const float CAMERA_FAR_PLANE = 500.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 250.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;

// Movement
const float MOVEMENT_SPEED = 150.0f;
const float MOUSE_SENSITIVITY = 0.1f;
bool isFirstMouseCallback = true;
float lastMouseX = Constants::WIDTH / 2.0;
float lastMouseY = Constants::HEIGHT / 2.0;

// Timing
float deltaTime = 0.0f;
float lastFrameTimestamp = 0.0f;

void processInput(GLFWwindow *window) {
    float cameraSpeed = static_cast<float>(MOVEMENT_SPEED * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraDirection;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraDirection;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
    }
}

void mouseCallback(GLFWwindow *_, double dMouseX, double dMouseY) {
    float mouseX = static_cast<float>(dMouseX);
    float mouseY = static_cast<float>(dMouseY);

    float offsetX = 0.0f, offsetY = 0.0f;
    if (isFirstMouseCallback) {
        isFirstMouseCallback = false;
    } else {
        offsetX = mouseX - lastMouseX;
        offsetY = lastMouseY - mouseY;
    }
    cameraYaw += offsetX * MOUSE_SENSITIVITY;
    cameraPitch += offsetY * MOUSE_SENSITIVITY;

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Limit the cameraPitch
    if (cameraPitch > 89.0f) {
        cameraPitch = 89.0f;
    } else if (cameraPitch < -89.0f) {
        cameraPitch = -89.0f;
    }

    cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection.y = sin(glm::radians(cameraPitch));
    cameraDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection = glm::normalize(cameraDirection);
}

GLFWwindow *initializeWindow() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(
            Constants::WIDTH, Constants::HEIGHT,
            "GraficaPeCalc - proiect 2",
            nullptr, nullptr
    );
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    // Mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    glewInit();
    glfwSwapInterval(1);

    return window;
}

void initializeShaders() {
    shaderProgram = ShadersUtils::loadShaders(
            "../src/shaders/shader.vert",
            "../src/shaders/shader.frag"
    );

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

    // Projection
    glm::mat4 projection = glm::perspective(
            glm::radians(CAMERA_FOV),
            (float) Constants::WIDTH / (float) Constants::HEIGHT,
            CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE
    );
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

    // View
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

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

        // Input
        processInput(window);

        // Timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrameTimestamp;
        lastFrameTimestamp = currentFrame;

        // Render
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