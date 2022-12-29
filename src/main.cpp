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
const float CAMERA_FAR_PLANE = 3500.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 300.0f, -1000.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;

// Movement
const float MOVEMENT_SPEED = 200.0f;
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
        cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos.y += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPos.y -= cameraSpeed;
    }
}

void mouseCallback(GLFWwindow *_, double dMouseX, double dMouseY) {
    float mouseX = static_cast<float>(dMouseX);
    float mouseY = static_cast<float>(dMouseY);

    float offsetX = 0.0f, offsetY = 0.0f;
    if (isFirstMouseCallback) {
        isFirstMouseCallback = false;
    } else {
        offsetX = lastMouseX - mouseX;
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
            //            Coordinates             |             Colors          | Shininess
            // Grass
            /* 0 (Grass - 43) */
            -1029.73f, 0.0f, -920.41f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 1 (Grass - 32) */
            -96.5f, 0.0f, -920.41f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 2 (Grass - 65) */
            -1029.73f, 0.0f, 918.24f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 3 (Grass - 30) */
            -96.5f, 0.0f, -64.89f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 4 (Grass - 64) */
            880.5f, 0.0f, 918.24f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 5 (Grass - 25) */
            366.22f, 0.0f, -64.89f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 6 (Grass - 34) */
            366.22f, 0.0f, -920.41f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 7 (Grass - 58) */
            880.5f, 0.0f, -920.41f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 8 (Grass - 28) */
            47.62f, 0.0f, -40.76f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 9 (Grass - 50) */
            -1029.73f, -48.1f, -920.41f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 10 (Grass - 59) */
            880.5f, -48.1f, -920.41f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 11 (Grass - 66) */
            880.5f, -48.1f, 918.24f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 12 (Grass - 67) */
            -1029.73f, -48.1f, 918.24f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 13 (Grass - 60) */
            -1029.73f, 0.0f, 918.24f,
            Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b, Constants::SHININESS_GRASS,
            /* 14 (Road - 32) */
            -96.5f, 0.0f, -920.41f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 15 (Road - 30) */
            -96.5f, 0.0f, -64.89f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 16 (Road - 34) */
            366.22f, 0.0f, -920.41f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 17 (Road - 25) */
            366.22f, 0.0f, -64.89f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 18 (Road - 29) */
            47.62f, 5.0f, -104.89f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 19 (Road - 28) */
            47.62f, 5.0f, -40.76f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 20 (Road - 35) */
            233.2f, 5.0f, -104.89f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 21 (Road - 27) */
            233.2f, 5.0f, -40.76f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 22 ( - ) */
            0.0f, 0.0f, 0.0f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 23 ( - ) */
            0.0f, 0.0f, 0.0f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
            /* 24 ( - ) */
            0.0f, 0.0f, 0.0f,
            Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b, Constants::SHININESS_ROAD,
    };
    GLuint indices[] = {
            // Grass
            0, 1, 2, // 43, 32, 65
            1, 2, 3, // 32, 65, 30
            3, 2, 4, // 30, 65, 64
            3, 4, 5, // 30, 64, 25
            5, 4, 7, // 25, 64, 58
            5, 7, 6, // 25, 58, 34
            0, 9, 7, // 43, 50, 58
            9, 7, 10, // 50, 58, 59
            7, 10, 4, // 58, 59, 64
            7, 10, 11, // 58, 59, 66
            7, 11, 4, // 58, 66, 64
            4, 11, 2, // 64, 66, 65
            11, 2, 12, // 66, 65, 67
            2, 12, 9, // 65, 67, 50
            9, 2, 0, // 50, 65, 43
            11, 12, 9, // 66, 67, 50
            9, 11, 10,// 50, 66, 59
            // Road
            14, 15, 16, // 32, 30, 34
            15, 16, 17, // 30, 34, 25
            18, 19, 20, // 29, 28, 35,
            19, 20, 21, // 28, 35, 27
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1); // 1 = color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // 1 = shininess
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
}

void render() {
    glUseProgram(shaderProgram);

    // Projection
    glm::mat4 projection = glm::perspectiveLH(
            glm::radians(CAMERA_FOV),
            (float) Constants::WIDTH / (float) Constants::HEIGHT,
            CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE
    );
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

    // View
    glm::mat4 view = glm::lookAtLH(cameraPos, cameraPos + cameraDirection, cameraUp);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
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
        glClearColor(Constants::COLOR_SKY.r, Constants::COLOR_SKY.g, Constants::COLOR_SKY.b, 1.0f);
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