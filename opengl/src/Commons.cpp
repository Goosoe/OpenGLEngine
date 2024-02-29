#include "Commons.h"
#include "Camera.h"
#include <array>
#include <iostream>
#include <string>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
 * Key values for the keys that must store data about the previous type of input.
 * Used as index access to previousKeyValues
 */
enum KeyValues {
    Z = 0,
    TOTAL
};

Camera* cam;
float lastX = SCR_WIDTH / 2.f;
float lastY = SCR_HEIGHT / 2.f; 
bool wireframeMode = false;

std::array<uint8_t, TOTAL> previousKeyValues {GLFW_RELEASE};

void setupWindowData(Camera* camera, float xVal, float yVal)
{
    cam = camera;
    lastX = xVal;
    lastY = yVal;
}
//TODO: Make this camelcase
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void handleKeyboardInput(GLFWwindow* window, float deltaTime)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /** MOVEMENT **/

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->ProcessKeyboard(Camera_Movement::UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam->ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

    /** CHANGE SETTINGS **/

    uint8_t keyVal = glfwGetKey(window, GLFW_KEY_Z); 
    //Wireframe toggle
    if (keyVal == GLFW_RELEASE && previousKeyValues[Z] == GLFW_PRESS)
    {
        wireframeMode = !wireframeMode;
        if(wireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    previousKeyValues[Z] = keyVal;
}

/**
* Adjusts the cursor to the center. It then sets the default mouse callback as a cursor callback.
* Using this to avoid the if statement every callback.
*/
void mouseCallbackAdjustCursor(GLFWwindow* window, double xposIn, double yposIn)
{
    lastX = xposIn;
    lastY = yposIn;
    float xoffset = xposIn - lastX;
    float yoffset = lastY - yposIn; 
    lastX = xposIn;
    lastY = yposIn;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->ProcessMouseMovement(xoffset, yoffset);

    glfwSetCursorPosCallback(window, defaultMouseCallback);
}
void defaultMouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    cam->ProcessMouseMovement(xoffset, yoffset);
}

void printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        std::string errorString;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString.c_str());
    }
}

unsigned int textureFromFile(const char *filename, const std::string &directory)
{
    std::string fullPath = std::string(filename);
    fullPath = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
    {
        std::cout << "Texture failed to load at path: " << fullPath << std::endl;
        stbi_image_free(data);
    }
    GLenum format;
    if (nrComponents == 1)
    {
        format = GL_RED;
    }
    else if (nrComponents == 3)
    {
        format = GL_RGB;
    }
    else //if (nrComponents == 4)
    {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}
