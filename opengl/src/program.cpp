// Local headers
#include "program.h"
#include "GLFW/glfw3.h"
#include "Entity.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float4.hpp"
#include "utils/Camera.h"
#include "utils/window.h"
#include "utils/Model.h"
#include "utils/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

//TODO: put this in a better place
// settings
Camera camera(glm::vec3(1, 1, 5));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

constexpr float ambientLight = .1f;
constexpr float specularVal = 0.8f;


//Scene data
const glm::vec3 lightColor (1.f, 1.f, 1.f);
const glm::vec3 objColor (0.5f, 0.f, 0.f);
const glm::vec3 testObjColor (0.5f, 0.5f, 0.f);

//TODO: add resizable window feature
glm::mat4 projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH/ SCR_HEIGHT, 0.1f, 100.f);

void runProgram(GLFWwindow* window)
{
    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // camera
    glm::mat4 view(1.f);
    const glm::vec3 lightPos (2.f);

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float prevTime = (float) glfwGetTime();


    //setup data vectors
    std::vector<Model> models;
    //for normal shaders that do not emit light
    std::vector<ShaderData> shaders;
    //for shaders that emit light
    std::vector<ShaderData> illuminationShaders;

    {
        //lightShader
        ShaderData lightShader;
        lightShader.program = Shader::createProgram();
        Shader::makeBasicShader(lightShader, "./opengl/shaders/LightV.glsl", "./opengl/shaders/LightF.glsl");

        glUseProgram(lightShader.program);
        Shader::setVec3(lightShader.program, "color", lightColor);
        glUseProgram(0);

        illuminationShaders.emplace_back(lightShader);

        //basic shader
        ShaderData basicShader;
        basicShader.program = Shader::createProgram();
        Shader::makeBasicShader(basicShader, "./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");

        glUseProgram(basicShader.program);
        Shader::setVec3(basicShader.program, "lightPos", lightPos);
        Shader::setVec3(basicShader.program, "lightColor", lightColor);
        Shader::setVec3(basicShader.program, "objColor", objColor);
        Shader::setFloat(basicShader.program, "ambientVal", ambientLight);
        Shader::setFloat(basicShader.program, "specularVal", specularVal);
        glUseProgram(0);

        shaders.emplace_back(basicShader);

        //extra testing shader to test if system is working properly. Since it uses the same shader, we could have a simple function
        //that changed the uniforms for the values we wanted. Quicker and cheaper solution
        ShaderData testShader;
        testShader.program = Shader::createProgram();
        Shader::makeBasicShader(testShader, "./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");

        glUseProgram(testShader.program);
        Shader::setVec3(testShader.program, "lightPos", lightPos);
        Shader::setVec3(testShader.program, "lightColor", lightColor);
        Shader::setVec3(testShader.program, "objColor", testObjColor);
        Shader::setFloat(testShader.program, "ambientVal", ambientLight);
        Shader::setFloat(testShader.program, "specularVal", specularVal);
        glUseProgram(0);

        shaders.emplace_back(basicShader);
        //====

        models.emplace_back("./opengl/models/teapot.stl");
        Model& teapot = models.back();
        
        teapot.addEntity(basicShader.program, projection, glm::vec3(0.3f, 0.3f, 0.3f));

        teapot.addEntity(testShader.program, projection, glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(2.f, 0.f, 0.f));

        teapot.addEntity(lightShader.program, projection, glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(2.f));
    }

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        float currTime = (float) glfwGetTime();
        float deltaTime = currTime - prevTime;

        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here

        // update view matrix with updated camera data
        view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

        updateUniformsOfShaders(shaders);
        
        for(size_t i = 0; i < models.size(); i++)
        { 
            models[i].drawEntities(view);
        }
        prevTime = currTime;
        
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
    // Use escape key for terminating the GLFW window
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void updateUniformsOfShaders(const std::vector<ShaderData>& shaders)
{
    for(size_t i = 0; i < shaders.size(); i++)
    { 
        GLuint program = shaders[i].program; 
        glUseProgram(program);
        Shader::setVec3(program, "cameraPos", camera.Position);
        glUseProgram(0);
    }
}
