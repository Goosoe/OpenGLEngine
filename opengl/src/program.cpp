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
#include "utils/DefaultObjects.h"
#include "utils/window.h"
#include "utils/Shader.h"
//#include "utils/TextureLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

//TODO: make this in a bettre place
// settings
Camera camera(glm::vec3(0, 0, 3));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float ambientLight = 0.8f;
//bool firstMouse = true;

//Scene data
const glm::vec3 lightColor (1.f, 1.f, 1.f);
const glm::vec3 objColor (1.f, 1.f, 1.f);
//const glm::vec3 objColor (0.8f, 0.3f, 0.3f);

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

    //const data setup

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Scene setup

    //texture loading
//    texture::loadTexture(GL_TEXTURE0, "./textures/wall.jpg");
//    texture::loadTexture(GL_TEXTURE1, "./textures/banana.png");

    //buffers setup
   // unsigned int VAO, VBO, EBO;

   // glGenVertexArrays(1, &VAO);
   // glGenBuffers(1, &VBO);
   // glGenBuffers(1, &EBO);

   // glBindVertexArray(VAO);

   // glBindBuffer(GL_ARRAY_BUFFER, VBO);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultCube::vertices), DefaultCube::vertices, GL_STATIC_DRAW);

   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

   // //glBindBuffer(GL_ARRAY_BUFFER, 0); 
   // glEnableVertexAttribArray(0);  

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DefaultCube::indices), DefaultCube::indices, GL_STATIC_DRAW);
     
    //shader program setup
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f)); 

   // Entity lightModel(projection, glm::vec3(1.f, 1.0f, 1.0f), glm::vec3(1.f));
   // Entity objModel(projection);
    
   // objModel.shader.makeBasicShader("./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");
   // lightModel.shader.makeBasicShader("./opengl/shaders/LightV.glsl", "./opengl/shaders/LightF.glsl");

    //Shader basic; 
    //basic.makeBasicShader("./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");

   // //light shader
   // lightModel.shader.activate();
   // glUniform4fv(glGetUniformLocation(lightModel.shader.get(), "color"), 1, &lightColor[0]);

    float prevTime = (float) glfwGetTime();

    Entity teapot("./opengl/models/teapot.stl", projection);
    teapot.shader.makeBasicShader("./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");
    //
   // teapot.model.
   // teapot.shader.
    //Model model("./opengl/models/teapot.stl");
    //model.
    //Model model2("./opengl/models/teapot.stl");
    
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

        // light shader
       // lightModel.shader.activate();
       // lightModel.updateCameraUniforms(view);

       // glUniform3fv(glGetUniformLocation(objModel.shader.get(), "objColor"), 1, glm::value_ptr(objColor));
       // glUniform3fv(glGetUniformLocation(objModel.shader.get(), "lightColor"), 1, glm::value_ptr(lightColor));
       // glUniform1f(glGetUniformLocation(objModel.shader.get(), "ambient"), ambientLight);

        //SHADER MUST BE ACTIVE BEFORE SETTING UNIFORMS
        teapot.shader.activate();

        teapot.shader.setVec3("objColor", objColor);
        teapot.shader.setVec3("lightColor", lightColor);
        teapot.shader.setFloat("ambient", ambientLight);

        teapot.draw();
        teapot.updateCameraUniforms(view);

        teapot.shader.deactivate();

       // glUniform3fv(glGetUniformLocation(lightModel.shader.get(), "color"), 1, glm::value_ptr(lightColor));
       // //glUniform1f(glGetUniformLocation(lightModel.shader.get(), "ambient"), ambientLight);
       // glDrawElements(GL_TRIANGLES, DefaultCube::cubeVertices, GL_UNSIGNED_INT, 0);

        // object shader 
       // objModel.shader.activate();
        //model.Draw(objModel.shader);
       // objModel.updateCameraUniforms(view);

       // glUniform3fv(glGetUniformLocation(objModel.shader.get(), "objColor"), 1, glm::value_ptr(objColor));
       // glUniform3fv(glGetUniformLocation(objModel.shader.get(), "lightColor"), 1, glm::value_ptr(lightColor));
       // glUniform1f(glGetUniformLocation(objModel.shader.get(), "ambient"), ambientLight);
       // glDrawElements(GL_TRIANGLES, DefaultCube::cubeVertices, GL_UNSIGNED_INT, 0);

        prevTime = currTime;
        
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }

    //lightModel.shader.deactivate();
    //objModel.shader.deactivate();
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

   // if (firstMouse)
   // {
   //     lastX = xpos;
   //     lastY = ypos;
   //     firstMouse = false;
   // }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
