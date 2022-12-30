#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/render/ShadersUtils.h"
#include "utils/Constants.h"

using namespace glm;
using namespace std;

GLuint shaderProgram;
GLuint vao, vbo, ebo;

// Locations - camera
GLuint viewLocation, projLocation;
// Locations - lighting
GLuint viewPositionLocation, lightPositionLocation, lightColorLocation, skyColorLocation;

// Camera
const float CAMERA_FOV = 75.0f;
const float CAMERA_NEAR_PLANE = 0.1f;
const float CAMERA_FAR_PLANE = 3500.0f;
glm::vec3 cameraPos = glm::vec3(100.0f, 300.0f, -1500.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = 90.0f;
float cameraPitch = 0.0f;

// Movement
const float MOVEMENT_SPEED = 400.0f;
const float MOUSE_SENSITIVITY = 0.1f;
bool isFirstMouseCallback = true;
float lastMouseX = Constants::WIDTH / 2.0;
float lastMouseY = Constants::HEIGHT / 2.0;

// Timing
float deltaTime = 0.0f;
float lastFrameTimestamp = 0.0f;

// Lighting
const glm::vec3 LIGHT_COLOR = glm::vec3(0.6f, 0.6f, 0.6f);
glm::vec3 lightPosition = glm::vec3(500.f, 1000.f, -1000.f);

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
    viewPositionLocation = glGetUniformLocation(shaderProgram, "viewPosition");
    lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPosition");
    lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
    skyColorLocation = glGetUniformLocation(shaderProgram, "skyColor");
}

void initializeScene() {
    const vec3 vertices[] = {
            // Grass top
            /* 0 (Grass top - 43) */vec3(-1029.73f, 0.0f, -920.41f),
            /* 1 (Grass top - 32) */vec3(-96.5f, 0.0f, -920.41f),
            /* 2 (Grass top - 65) */vec3(-1029.73f, 0.0f, 918.24f),
            /* 3 (Grass top - 30) */vec3(-96.5f, 0.0f, -64.89f),
            /* 4 (Grass top - 64) */vec3(880.5f, 0.0f, 918.24f),
            /* 5 (Grass top - 25) */vec3(366.22f, 0.0f, -64.89f),
            /* 6 (Grass top - 34) */vec3(366.22f, 0.0f, -920.41f),
            /* 7 (Grass top - 58) */vec3(880.5f, 0.0f, -920.41f),
            /* 8 (Grass top - 28) */vec3(47.62f, 0.0f, -40.76f),
            /* 9 (Grass top - 29) */vec3(47.62f, 0.0f, -64.89f),
            /* 10 (Grass top - 27) */vec3(233.2f, 0.0f, -40.76f),
            /* 11 (Grass top - 35) */vec3(233.2f, 0.0f, -64.89f),
            // Road top
            /* 12 (Road top - 25) */vec3(366.22f, 0.0f, -64.89f),
            /* 13 (Road top - 27) */vec3(233.2f, 0.0f, -40.76f),
            /* 14 (Road top - 28) */vec3(47.62f, 0.0f, -40.76f),
            /* 15 (Road top - 29) */vec3(47.62f, 0.0f, -64.89f),
            /* 16 (Road top - 30) */vec3(-96.5f, 0.0f, -64.89f),
            /* 17 (Road top - 32) */vec3(-96.5f, 0.0f, -920.41f),
            /* 18 (Road top - 34) */vec3(366.22f, 0.0f, -920.41f),
            /* 19 (Road top - 35) */vec3(233.2f, 0.0f, -64.89f),
            // Grass front
            /* 20 (Grass front - 43) */vec3(-1029.73f, 0.0f, -920.41f),
            /* 21 (Grass front - 50) */vec3(-1029.73f, -48.1f, -920.41f),
            /* 22 (Grass front - 58) */vec3(880.5f, 0.0f, -920.41f),
            /* 23 (Grass front - 59) */vec3(880.5f, -48.1f, -920.41f),
            // Grass right
            /* 24 (Grass right - 58) */vec3(880.5f, 0.0f, -920.41f),
            /* 25 (Grass right - 59) */vec3(880.5f, -48.1f, -920.41f),
            /* 26 (Grass right - 64) */vec3(880.5f, 0.0f, 918.24f),
            /* 27 (Grass right - 66) */vec3(880.5f, -48.1f, 918.24f),
            // Grass back
            /* 28 (Grass back - 64) */vec3(880.5f, 0.0f, 918.24f),
            /* 29 (Grass back - 65) */vec3(-1029.73f, 0.0f, 918.24f),
            /* 30 (Grass back - 66) */vec3(880.5f, -48.1f, 918.24f),
            /* 31 (Grass back - 67) */vec3(-1029.73f, -48.1f, 918.24f),
            // Grass left
            /* 32 (Grass left - 43) */vec3(-1029.73f, 0.0f, -920.41f),
            /* 33 (Grass left - 50) */vec3(-1029.73f, -48.1f, -920.41f),
            /* 34 (Grass left - 65) */vec3(-1029.73f, 0.0f, 918.24f),
            /* 35 (Grass left - 67) */vec3(-1029.73f, -48.1f, 918.24f),
            // Grass bottom
            /* 36 (Grass bottom - 50) */vec3(-1029.73f, -48.1f, -920.41f),
            /* 37 (Grass bottom - 59) */vec3(880.5f, -48.1f, -920.41f),
            /* 38 (Grass bottom - 66) */vec3(880.5f, -48.1f, 918.24f),
            /* 39 (Grass bottom - 67) */vec3(-1029.73f, -48.1f, 918.24f),
            // House front
            /* 40 (House front - 50) */vec3(149.63f, 848.02f, -64.89f),
            /* 41 (House front - 51) */vec3(62.39f, 678.83f, -64.89f),
            /* 42 (House front - 54) */vec3(62.39f, 453.65f, -64.89f),
            /* 43 (House front - 56) */vec3(31.46f, 316.43f, -64.89f),
            /* 44 (House front - 58) */vec3(31.46f, 0.0f, -64.89f),
            /* 45 (House front - 59) */vec3(223.28f, 678.83f, -64.89f),
            /* 46 (House front - 81) */vec3(-224.47f, 466.78f, -64.89f),
            /* 47 (House front - 62) */vec3(483.08f, 464.99f, -64.89f),
            /* 48 (House front - 63) */vec3(249.36f, 316.43f, -64.89f),
            /* 49 (House front - 64) */vec3(249.36f, 0.0f, -64.89f),
            /* 50 (House front - 65) */vec3(483.08f, 0.0f, -64.89f),
            /* 51 (House front - 79) */vec3(-224.47f, 0.0f, -64.89f),
            /* 52 (House front - 55) */vec3(223.28f, 453.65f, -64.89f),
            // House right
            /* 53 (House right - 27) */vec3(483.08f, 464.99f, 636.31f),
            /* 54 (House right - 29) */vec3(483.08f, 0.0f, 636.31f),
            /* 55 (House right - 62) */vec3(483.08f, 464.99f, -64.89f),
            /* 56 (House right - 65) */vec3(483.08f, 0.0f, -64.89f),
            // House back
            /* 57 (House back - 27) */vec3(483.08f, 464.99f, 636.31f),
            /* 58 (House back - 29) */vec3(483.08f, 0.0f, 636.31f),
            /* 59 (House back - 30) */vec3(149.63f, 848.02f, 636.31f),
            /* 60 (House back - 74) */vec3(-224.47f, 466.78f, 636.31f),
            /* 61 (House back - 76) */vec3(-224.47f, 0.0f, 636.31f),
            // House left
            /* 62 (House left - 74) */vec3(-224.47f, 466.78f, 636.31f),
            /* 63 (House left - 76) */vec3(-224.47f, 0.0f, 636.31f),
            /* 64 (House left - 79) */vec3(-224.47f, 0.0f, -64.89f),
            /* 65 (House left - 81) */vec3(-224.47f, 466.78f, -64.89f),
            // Door
            /* 66 (Door - 4) */vec3(47.62f, 300.27f, -40.76f),
            /* 67 (Door - 5) */vec3(233.2f, 300.27f, -40.76f),
            /* 68 (Door - 6) */vec3(47.62f, 0.0f, -40.76f),
            /* 69 (Door - 7) */vec3(233.2f, 0.0f, -40.76f),
            // Door frame - front
            /* 70 (Door frame - front - 23) */vec3(249.36f, 0.0f, -77.59f),
            /* 71 (Door frame - front - 29) */vec3(249.36f, 316.43f, -77.59f),
            /* 72 (Door frame - front - 38) */vec3(31.46f, 0.0f, -77.59f),
            /* 73 (Door frame - front - 39) */vec3(31.46f, 316.43f, -77.59f),
            /* 74 (Door frame - front - 46) */vec3(47.62f, 300.27f, -77.59f),
            /* 75 (Door frame - front - 47) */vec3(47.62f, 0.0f, -77.59f),
            /* 76 (Door frame - front - 54) */vec3(233.2f, 0.0f, -77.59f),
            /* 77 (Door frame - front - 55) */vec3(233.2f, 300.27f, -77.59f),
            // Door frame - top
            /* 78 (Door frame - top - 28) */vec3(249.36f, 316.43f, -64.89f),
            /* 79 (Door frame - top - 29) */vec3(249.36f, 316.43f, -77.59f),
            /* 80 (Door frame - top - 37) */vec3(31.46f, 316.43f, -64.89f),
            /* 81 (Door frame - top - 39) */vec3(31.46f, 316.43f, -77.59f),
            // Door frame - left
            /* 82 (Door frame - left - 36) */vec3(31.46f, 0.0f, -64.89f),
            /* 83 (Door frame - left - 37) */vec3(31.46f, 316.43f, -64.89f),
            /* 84 (Door frame - left - 38) */vec3(31.46f, 0.0f, -77.59f),
            /* 85 (Door frame - left - 39) */vec3(31.46f, 316.43f, -77.59f),
            /* 86 (Door frame - left - 52) */vec3(233.2f, 0.0f, -40.76f),
            /* 87 (Door frame - left - 53) */vec3(233.2f, 300.27f, -40.76f),
            /* 88 (Door frame - left - 54) */vec3(233.2f, 0.0f, -77.59f),
            /* 89 (Door frame - left - 55) */vec3(233.2f, 300.27f, -77.59f),
            // Door frame - right
            /* 90 (Door frame - right - 21) */vec3(249.36f, 0.0f, -64.89f),
            /* 91 (Door frame - right - 23) */vec3(249.36f, 0.0f, -77.59f),
            /* 92 (Door frame - right - 28) */vec3(249.36f, 316.43f, -64.89f),
            /* 93 (Door frame - right - 29) */vec3(249.36f, 316.43f, -77.59f),
            /* 94 (Door frame - right - 44) */vec3(47.62f, 300.27f, -40.76f),
            /* 95 (Door frame - right - 45) */vec3(47.62f, 0.0f, -40.76f),
            /* 96 (Door frame - right - 46) */vec3(47.62f, 300.27f, -77.59f),
            /* 97 (Door frame - right - 47) */vec3(47.62f, 0.0f, -77.59f),
            // Door frame - top
            /* 98 (Door frame - top - 44) */vec3(47.62f, 300.27f, -40.76f),
            /* 99 (Door frame - top - 46) */vec3(47.62f, 300.27f, -77.59f),
            /* 100 (Door frame - top - 53) */vec3(233.2f, 300.27f, -40.76f),
            /* 101 (Door frame - top - 55) */vec3(233.2f, 300.27f, -77.59f),
            // Window - bottom left
            /* 102 (Window - bottom left - 4) */vec3(75.52f, 559.77f, -40.76f),
            /* 103 (Window - bottom left - 5) */vec3(135.49f, 559.77f, -40.76f),
            /* 104 (Window - bottom left - 6) */vec3(75.52f, 466.78f, -40.76f),
            /* 105 (Window - bottom left - 7) */vec3(135.49f, 466.78f, -40.76f),
            // Window - bottom right
            /* 106 (Window - bottom right - 4) */vec3(150.1f, 559.77f, -40.76f),
            /* 107 (Window - bottom right - 5) */vec3(210.14f, 559.77f, -40.76f),
            /* 108 (Window - bottom right - 6) */vec3(150.1f, 466.78f, -40.76f),
            /* 109 (Window - bottom right - 7) */vec3(210.14f, 466.78f, -40.76f),
            // Window - top left
            /* 110 (Window - top left - 4) */vec3(75.52f, 665.7f, -40.76f),
            /* 111 (Window - top left - 5) */vec3(135.49f, 665.7f, -40.76f),
            /* 112 (Window - top left - 6) */vec3(75.52f, 572.41f, -40.76f),
            /* 113 (Window - top left - 7) */vec3(135.49f, 572.41f, -40.76f),
            // Window - top right
            /* 114 (Window - top right - 4) */vec3(150.1f, 665.7f, -40.76f),
            /* 115 (Window - top right - 5) */vec3(210.14f, 665.7f, -40.76f),
            /* 116 (Window - top right - 6) */vec3(150.1f, 572.41f, -40.76f),
            /* 117 (Window - top right - 7) */vec3(210.14f, 572.41f, -40.76f),
            // Window - inner frame - front
            /* 118 (Window - inner frame - front - 37) */vec3(150.1f, 559.77f, -59.17f),
            /* 119 (Window - inner frame - front - 39) */vec3(150.1f, 466.78f, -59.17f),
            /* 120 (Window - inner frame - front - 54) */vec3(135.49f, 466.78f, -59.17f),
            /* 121 (Window - inner frame - front - 55) */vec3(135.49f, 559.77f, -59.17f),
            /* 122 (Window - inner frame - front - 66) */vec3(75.52f, 559.77f, -59.17f),
            /* 123 (Window - inner frame - front - 81) */vec3(210.14f, 559.77f, -59.17f),
            /* 124 (Window - inner frame - front - 93) */vec3(210.14f, 572.41f, -59.17f),
            /* 125 (Window - inner frame - front - 102) */vec3(150.1f, 665.7f, -59.17f),
            /* 126 (Window - inner frame - front - 103) */vec3(150.1f, 572.41f, -59.17f),
            /* 127 (Window - inner frame - front - 111) */vec3(135.49f, 665.7f, -59.17f),
            /* 128 (Window - inner frame - front - 117) */vec3(135.49f, 572.41f, -59.17f),
            /* 129 (Window - inner frame - front - 119) */vec3(75.52f, 572.41f, -59.17f),
            // Window - inner frame - bottom
            /* 130 (Window - inner frame - bottom - 21) */vec3(150.1f, 466.78f, -40.76f),
            /* 131 (Window - inner frame - bottom - 39) */vec3(150.1f, 466.78f, -59.17f),
            /* 132 (Window - inner frame - bottom - 52) */vec3(135.49f, 466.78f, -40.76f),
            /* 133 (Window - inner frame - bottom - 54) */vec3(135.49f, 466.78f, -59.17f),
            /* 134 (Window - inner frame - bottom - 65) */vec3(75.52f, 466.78f, -40.76f),
            /* 135 (Window - inner frame - bottom - 67) */vec3(75.52f, 466.78f, -77.59f),
            /* 136 (Window - inner frame - bottom - 82) */vec3(210.14f, 466.78f, -40.76f),
            /* 137 (Window - inner frame - bottom - 84) */vec3(210.14f, 466.78f, -77.59f),
            /* 138 (Window - inner frame - bottom - 92) */vec3(210.14f, 572.41f, -40.76f),
            /* 139 (Window - inner frame - bottom - 93) */vec3(210.14f, 572.41f, -59.17f),
            /* 140 (Window - inner frame - bottom - 101) */vec3(150.1f, 572.41f, -40.76f),
            /* 141 (Window - inner frame - bottom - 103) */vec3(150.1f, 572.41f, -59.17f),
            /* 142 (Window - inner frame - bottom - 116) */vec3(135.49f, 572.41f, -40.76f),
            /* 143 (Window - inner frame - bottom - 117) */vec3(135.49f, 572.41f, -59.17f),
            /* 144 (Window - inner frame - bottom - 118) */vec3(75.52f, 572.41f, -40.76f),
            /* 145 (Window - inner frame - bottom - 119) */vec3(75.52f, 572.41f, -59.17f),
            // Window - inner frame - left
            /* 146 (Window - inner frame - left - 20) */vec3(150.1f, 559.77f, -40.76f),
            /* 147 (Window - inner frame - left - 21) */vec3(150.1f, 466.78f, -40.76f),
            /* 148 (Window - inner frame - left - 37) */vec3(150.1f, 559.77f, -59.17f),
            /* 149 (Window - inner frame - left - 39) */vec3(150.1f, 466.78f, -59.17f),
            /* 150 (Window - inner frame - left - 64) */vec3(75.52f, 559.77f, -40.76f),
            /* 151 (Window - inner frame - left - 65) */vec3(75.52f, 466.78f, -40.76f),
            /* 152 (Window - inner frame - left - 66) */vec3(75.52f, 559.77f, -59.17f),
            /* 153 (Window - inner frame - left - 67) */vec3(75.52f, 466.78f, -77.59f),
            /* 154 (Window - inner frame - left - 68) */vec3(75.52f, 665.7f, -77.59f),
            /* 155 (Window - inner frame - left - 71) */vec3(75.52f, 665.7f, -40.76f),
            /* 156 (Window - inner frame - left - 100) */vec3(150.1f, 665.7f, -40.76f),
            /* 157 (Window - inner frame - left - 101) */vec3(150.1f, 572.41f, -40.76f),
            /* 158 (Window - inner frame - left - 102) */vec3(150.1f, 665.7f, -59.17f),
            /* 159 (Window - inner frame - left - 103) */vec3(150.1f, 572.41f, -59.17f),
            /* 160 (Window - inner frame - left - 118) */vec3(75.52f, 572.41f, -40.76f),
            /* 161 (Window - inner frame - left - 119) */vec3(75.52f, 572.41f, -59.17f),
            // Window - inner frame - right
            /* 162 (Window - inner frame - right - 52) */vec3(135.49f, 466.78f, -40.76f),
            /* 163 (Window - inner frame - right - 53) */vec3(135.49f, 559.77f, -40.76f),
            /* 164 (Window - inner frame - right - 54) */vec3(135.49f, 466.78f, -59.17f),
            /* 165 (Window - inner frame - right - 55) */vec3(135.49f, 559.77f, -59.17f),
            /* 166 (Window - inner frame - right - 80) */vec3(210.14f, 559.77f, -40.76f),
            /* 167 (Window - inner frame - right - 81) */vec3(210.14f, 559.77f, -59.17f),
            /* 168 (Window - inner frame - right - 82) */vec3(210.14f, 466.78f, -40.76f),
            /* 169 (Window - inner frame - right - 84) */vec3(210.14f, 466.78f, -77.59f),
            /* 170 (Window - inner frame - right - 86) */vec3(210.14f, 665.7f, -40.76f),
            /* 171 (Window - inner frame - right - 87) */vec3(210.14f, 665.7f, -77.59f),
            /* 172 (Window - inner frame - right - 92) */vec3(210.14f, 572.41f, -40.76f),
            /* 173 (Window - inner frame - right - 93) */vec3(210.14f, 572.41f, -59.17f),
            /* 174 (Window - inner frame - right - 109) */vec3(135.49f, 665.7f, -40.76f),
            /* 175 (Window - inner frame - right - 111) */vec3(135.49f, 665.7f, -59.17f),
            /* 176 (Window - inner frame - right - 116) */vec3(135.49f, 572.41f, -40.76f),
            /* 177 (Window - inner frame - right - 117) */vec3(135.49f, 572.41f, -59.17f),
            // Window - outer frame - front
            /* 178 (Window - outer frame - front - 9) */vec3(75.52f, 665.7f, -77.59f),
            /* 179 (Window - outer frame - front - 11) */vec3(75.52f, 466.78f, -77.59f),
            /* 180 (Window - outer frame - front - 12) */vec3(210.14f, 466.78f, -77.59f),
            /* 181 (Window - outer frame - front - 14) */vec3(210.14f, 665.7f, -77.59f),
            /* 182 (Window - outer frame - front - 22) */vec3(62.39f, 453.65f, -77.59f),
            /* 183 (Window - outer frame - front - 31) */vec3(223.28f, 453.65f, -77.59f),
            /* 184 (Window - outer frame - front - 37) */vec3(223.28f, 678.83f, -77.59f),
            /* 185 (Window - outer frame - front - 39) */vec3(62.39f, 678.83f, -77.59f),
            // Window - inner frame - top
            /* 186 (Window - inner frame - top - 8) */vec3(210.14f, 665.7f, -40.76f),
            /* 187 (Window - inner frame - top - 9) */vec3(150.1f, 665.7f, -59.17f),
            /* 188 (Window - inner frame - top - 10) */vec3(210.14f, 665.7f, -77.59f),
            /* 189 (Window - inner frame - top - 11) */vec3(135.49f, 665.7f, -59.17f),
            /* 190 (Window - inner frame - top - 12) */vec3(75.52f, 665.7f, -77.59f),
            /* 191 (Window - inner frame - top - 13) */vec3(135.49f, 665.7f, -40.76f),
            /* 192 (Window - inner frame - top - 14) */vec3(75.52f, 665.7f, -40.76f),
            /* 193 (Window - inner frame - top - 15) */vec3(150.1f, 665.7f, -40.76f),
            // Window - outer frame - bottom & top
            // Window - outer frame - left & right
    };
    const glm::vec3 colors[] = {
            // Grass
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_ROAD.r, Constants::COLOR_ROAD.g, Constants::COLOR_ROAD.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_GRASS.r, Constants::COLOR_GRASS.g, Constants::COLOR_GRASS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_WALLS.r, Constants::COLOR_WALLS.g, Constants::COLOR_WALLS.b),
            vec3(Constants::COLOR_DOOR.r, Constants::COLOR_DOOR.g, Constants::COLOR_DOOR.b),
            vec3(Constants::COLOR_DOOR.r, Constants::COLOR_DOOR.g, Constants::COLOR_DOOR.b),
            vec3(Constants::COLOR_DOOR.r, Constants::COLOR_DOOR.g, Constants::COLOR_DOOR.b),
            vec3(Constants::COLOR_DOOR.r, Constants::COLOR_DOOR.g, Constants::COLOR_DOOR.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_WINDOWS.r, Constants::COLOR_WINDOWS.g, Constants::COLOR_WINDOWS.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
            vec3(Constants::COLOR_FRAMES.r, Constants::COLOR_FRAMES.g, Constants::COLOR_FRAMES.b),
    };
    const GLfloat shininess[] = {
            // Grass
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_ROAD,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_GRASS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_WALLS,
            Constants::SHININESS_DOOR,
            Constants::SHININESS_DOOR,
            Constants::SHININESS_DOOR,
            Constants::SHININESS_DOOR,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_WINDOWS,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
            Constants::SHININESS_FRAMES,
    };
    GLuint indices[] = {
            // Grass top
            1, 0, 2, // 32, 43, 65
            1, 2, 3, // 32, 65, 30
            3, 2, 4, // 30, 65, 64
            9, 3, 8, // 29, 30, 28
            8, 3, 4, // 28, 30, 64
            10, 8, 4, // 27, 28, 64
            5, 11, 10, // 25, 35, 27
            5, 10, 4,// 25, 27, 64
            7, 5, 4, // 58, 25, 64
            7, 6, 5, // 58, 34, 25
            // Road top
            18, 17, 16, // 34, 32, 30
            18, 16, 12, // 34, 30, 25
            19, 15, 14, // 35, 29, 28
            19, 14, 13, // 35, 28, 27
            // Grass front
            23, 21, 20, // 50, 50, 43
            23, 20, 22, // 59, 43, 58
            // Grass right
            27, 25, 24, // 66, 59, 58
            27, 24, 26, // 66, 58, 64
            // Grass back
            31, 30, 28, // 67, 66, 64
            31, 28, 29, // 67, 64, 65
            // Grass left
            33, 35, 34, // 50, 67, 65
            33, 34, 32, // 50, 65, 43
            // Grass bottom
            38, 39, 36, // 66, 67, 50
            38, 36, 37, // 66, 50, 59
            // House front
            44, 51, 43, // 58, 79, 56
            43, 51, 46, // 56, 79, 61
            42, 43, 46, // 54, 56, 81
            42, 46, 41, // 54, 81, 51
            41, 46, 40, // 61, 81, 50
            52, 43, 42, // 55, 56, 54
            47, 43, 52, // 62, 56, 55
            47, 48, 43, // 62, 63, 56
            47, 49, 48, // 62, 64, 63
            47, 50, 49, // 62, 65, 64
            47, 52, 45, // 62, 55, 59
            47, 45, 40, // 62, 59, 50
            45, 41, 40, // 59, 51, 50
            // House right
            54, 55, 56, // 29, 62, 65
            54, 53, 55, // 29, 62, 27
            // House back
            61, 58, 57, // 76, 29, 27
            61, 57, 60, // 76, 27, 74
            60, 57, 59, // 74, 27, 30
            // House left
            64, 63, 62, // 79, 76, 74
            64, 62, 65, // 79, 74, 81
            // Door
            67, 68, 66, // 5, 6, 4
            67, 69, 68, // 5, 7, 6
            // Door frame - front
            74, 75, 73, // 46, 47, 39
            73, 75, 72, // 39, 47, 38
            71, 74, 73, // 29, 46, 39
            71, 77, 74, // 29, 55, 46
            71, 76, 77, // 29, 54, 55
            71, 70, 76, // 29, 23, 54
            // Door frame - top
            79, 80, 81, // 29, 37, 39
            78, 80, 79, // 28, 29, 37
            // Door frame - left
            85, 84, 83, // 39, 38, 37
            83, 84, 82, // 37, 38, 36
            89, 88, 87, // 55, 54, 53
            87, 88, 86, // 53, 54, 52
            // Door frame - right
            90, 93, 91, // 21, 29, 23
            92, 90, 93, // 28, 21, 29
            96, 95, 97, // 46, 45, 47
            94, 95, 96, // 44, 45, 46
            // Door frame - top
            101, 100, 99, // 55, 53, 46
            100, 98, 99, // 53, 46, 44
            // Window - bottom left
            102, 105, 104, // 4, 7, 6
            103, 105, 102, // 5, 7, 4
            // Window - bottom right
            106, 109, 108, // 4, 7, 6
            107, 109, 106, // 5, 7, 4
            // Window - top left
            110, 113, 112, // 4, 7, 6
            111, 113, 110, // 5, 7, 4
            // Window - top right
            114, 117, 116, // 4, 7, 6
            115, 117, 114, // 5, 7, 4
            // Window - inner frame - front
            125, 127, 128, // 102, 111, 117
            125, 128, 126, // 102, 117, 103
            124, 129, 122, // 93, 119, 66
            124, 122, 121, // 93, 66, 55
            124, 121, 118, // 93, 55, 37
            124, 118, 123, // 93, 37, 81
            118, 121, 120, // 37, 55, 54
            118, 120, 119, // 37, 54, 39
            // Window - inner frame - bottom
            138, 139, 140, // 92, 93, 101
            141, 140, 139, // 93, 101, 103
            142, 143, 144, // 116, 117, 118
            145, 144, 143, // 117, 118, 119
            136, 137, 130, // 82, 84, 21
            131, 130, 137, // 84, 21, 39
            133, 131, 137, // 84, 39, 54
            135, 133, 137, // 84, 54, 67
            133, 134, 132, // 52, 54, 65
            135, 134, 133, // 54, 65, 67
            // Window - inner frame - left
            155, 161, 160,  // 71, 118, 119
            155, 161, 154, // 71, 119, 68
            152, 161, 154, // 66, 119, 68
            152, 153, 154, // 66, 67, 68
            152, 151, 153, // 66, 65, 67
            150, 151, 152, // 64, 65, 66
            156, 157, 158, // 100, 101, 102
            157, 159, 158, // 101, 103, 102
            146, 147, 148, // 20, 21, 37
            147, 149, 148, // 21, 39, 37
            // Window - inner frame - right
            175, 177, 174, // 111, 117, 109
            174, 177, 176, // 109, 117, 116
            165, 164, 163, // 55, 54, 53
            163, 164, 162, // 53, 54, 52
            171, 173, 170, // 87, 93, 86
            170, 173, 172, // 86, 93, 92
            171, 169, 173, // 87, 84, 93
            173, 169, 167, // 93, 84, 81
            167, 169, 168, // 81, 84, 82
            167, 168, 166, // 81, 82, 80
            // Window - outer frame - front
            184, 178, 185, // 37, 9, 39
            184, 181, 178, // 37, 14, 9
            184, 180, 181, // 37, 12, 14
            184, 183, 180, // 37, 31, 12
            183, 182, 180, // 31, 22, 12
            180, 182, 179, // 12, 22, 11
            179, 182, 185, // 11, 22, 39
            178, 179, 185, // 9, 11, 39
            // Window - inner frame - top
            191, 192, 190, // 13, 14, 12
            189, 191, 190, // 11, 13, 12
            188, 189, 190, // 10, 11, 12
            188, 187, 189, // 10, 9, 11
            188, 186, 187, // 10, 8, 9
            186, 193, 187, // 8, 15, 9
    };
    // Set the normals
    const int numNormals = sizeof(vertices) / sizeof(vec3);
    vec3 normals[numNormals];
    const int numIndices = sizeof(indices) / sizeof(GLuint);
    for (int i = 0; i < numIndices; i += 3) {
        glm::vec3 A = vertices[indices[i]];
        glm::vec3 B = vertices[indices[i + 1]];
        glm::vec3 C = vertices[indices[i + 2]];

        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 ABxAC = normalize(glm::cross(AB, AC));

        normals[indices[i]] = ABxAC;
        normals[indices[i + 1]] = ABxAC;
        normals[indices[i + 2]] = ABxAC;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(shininess) + sizeof(normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(shininess), shininess);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(shininess), sizeof(normals), normals);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // 0 = position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);
    glEnableVertexAttribArray(1); // 1 = color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) sizeof(vertices));
    glEnableVertexAttribArray(2); // 2 = shininess
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid *) (sizeof(vertices) + sizeof(colors)));
    glEnableVertexAttribArray(3); // 3 = normals
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) (sizeof(vertices) + sizeof(colors) + sizeof(shininess)));

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

    // Lighting
    glUniform3f(viewPositionLocation, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(lightColorLocation, LIGHT_COLOR.x, LIGHT_COLOR.y, LIGHT_COLOR.z);
    glUniform3f(skyColorLocation, Constants::COLOR_SKY.r, Constants::COLOR_SKY.g, Constants::COLOR_SKY.b);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 2000, GL_UNSIGNED_INT, 0);
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