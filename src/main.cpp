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

// Lighting
const glm::vec3 LIGHT_COLOR = glm::vec3(0.6f, 0.6f, 0.6f);
glm::vec3 lightPosition = glm::vec3(1000.f, 1500.f, 1000.f);

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
            // Grass
            // - Top
            /* 0 (Grass - 43) */vec3(-1029.73f, 0.0f, -920.41f),
            /* 1 (Grass - 32) */vec3(-96.5f, 0.0f, -920.41f),
            /* 2 (Grass - 65) */vec3(-1029.73f, 0.0f, 918.24f),
            /* 3 (Grass - 30) */vec3(-96.5f, 0.0f, -64.89f),
            /* 4 (Grass - 64) */vec3(880.5f, 0.0f, 918.24f),
            /* 5 (Grass - 25) */vec3(366.22f, 0.0f, -64.89f),
            /* 6 (Grass - 34) */vec3(366.22f, 0.0f, -920.41f),
            /* 7 (Grass - 58) */vec3(880.5f, 0.0f, -920.41f),
            // - Front
            // - Right
            // - Back
            // - Left
            // - Bottom
            /* ? ( - ) */vec3(0.0f, 0.0f, 0.0f),
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
    };
    GLuint indices[] = {
            // Grass
            // - Top
            1, 0, 2, // 32, 43, 65
            1, 2, 3, // 32, 65, 30
            3, 2, 4, // 30, 65, 64
            3, 4, 5, // 30, 64, 25
            5, 4, 7, // 25, 64, 58
            5, 7, 6, // 25, 58, 34
            // - Front
            // - Right
            // - Back
            // - Left
            // - Bottom
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
        glm::vec3 ABxAC = (glm::cross(AB, AC));

        normals[indices[i]] += ABxAC;
        normals[indices[i + 1]] += ABxAC;
        normals[indices[i + 2]] += ABxAC;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(shininess) + sizeof(normals), vertices, GL_STATIC_DRAW);
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