#pragma once
#include "GLFW/glfw3.h"
#include "Camera.h"
#include <glad/glad.h>
#include <string>

/** 
 * Has common functions and variables that are shared between levels
 */

// constants
constexpr int SCR_WIDTH = 1280;
constexpr int SCR_HEIGHT = 800;
constexpr char* WIND_TITLE = "OpenGL";
constexpr GLint WIND_RESIZABLE = GL_TRUE;
constexpr int WIND_SAMPLES = 4;

//Call this at the start of the level to set the required variables on window
void setupWindowData(Camera* camera, float xVal, float yVal);

// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window, float deltaTime);

// Callback function for recalculating window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
* Adjusts the cursor to the center. It then sets the default mouse callback as a cursor callback.
* Using this to avoid the if statement every callback.
*/
void mouseCallbackAdjustCursor(GLFWwindow* window, double xposIn, double yposIn);

// Default function for handling mouse movement
void defaultMouseCallback(GLFWwindow* window, double xposIn, double yposIn);

// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
void printGLError();

/**
 * Loads a texture from path and retuns the textureID
 */
unsigned int textureFromFile(const char *filename, const std::string &directory, glm::vec2& texDimensions);
