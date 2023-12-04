// Preprocessor directives
// todo: remove this?
#pragma once
#include "GLFW/glfw3.h"
#include "utils/Camera.h"

// System Headers
#include <glad/glad.h>

// Standard headers
#include <string>

// constants
constexpr int         SCR_WIDTH     = 1280;
constexpr int         SCR_HEIGHT    = 800;
const std::string WIND_TITLE     = "OpenGL";
constexpr GLint       WIND_RESIZABLE = GL_FALSE;
constexpr int         WIND_SAMPLES   = 4;


// === Holds the shared functions between levels ===

//Call this at the start of the level to set the required variables on window
void setupWindowData(Camera* camera, float xVal, float yVal);

// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window, float deltaTime);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Function for handling mouse movement
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
void printGLError();
