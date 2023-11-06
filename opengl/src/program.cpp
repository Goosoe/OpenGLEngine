// Local headers
#include "program.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utils/Camera.h"
#include "utils/window.h"
#include "utils/shader.h"
#include "utils/TextureLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

// settings
Utils::Camera camera(glm::vec3(0, 0, 3));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void runProgram(GLFWwindow* window)
{
    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // camera

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Scene setup

    //texture loading
    texture::loadTexture(GL_TEXTURE0, "./textures/wall.jpg");
    texture::loadTexture(GL_TEXTURE1, "./textures/banana.png");


    //const data setup
    constexpr float vertices[] = {
        //  coords(3)          RGBA(4)                 textureCoord(2)
        //FRONT
        -0.5f,  -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
        0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top right 
        
        //BACK
        -0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top right 
    };  

    constexpr unsigned int indices[] = {  
        //TOP
        1,3,2,
        1,2,0,

        //LEFT
        3,1,5,
        3,5,7,

        //RIGHT
        2,4,0,
        2,6,4,

        //BOTTOM
        7,5,6,
        6,5,4,

        //FRONT
        3,6,2,
        3,7,6,

        //BACK
        1,0,4,
        5,1,4
    };

    //buffers setup
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

    //glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glEnableVertexAttribArray(0);  
    glEnableVertexAttribArray(1);  
    glEnableVertexAttribArray(2);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
     
    //shader program setup
    Utils::Shader defaultShader;
    
    defaultShader.makeBasicShader("./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");
    defaultShader.activate();

    //setup the uniforms
    glUniform1i(glGetUniformLocation(defaultShader.get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(defaultShader.get(), "tex2"), 1);

    //setup the view
    glm::mat4 model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(-50.f), glm::vec3(1.0f, 0.0f, 0.0f)); 

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH/ SCR_HEIGHT, 0.1f, 100.f);

    GLuint modelId = glGetUniformLocation(defaultShader.get(), "model");
    GLuint viewId = glGetUniformLocation(defaultShader.get(), "view");
    GLuint projectionId = glGetUniformLocation(defaultShader.get(), "projection");

    float prevTime = (float) glfwGetTime();

   // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        // updating uniforms
        glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));

        float currTime = (float) glfwGetTime();
        float deltaTime = currTime - prevTime;
        model = glm::rotate(model, deltaTime * 2, glm::vec3(0,1,1));
        view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        prevTime = currTime;

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }

    defaultShader.deactivate();
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
        camera.ProcessKeyboard(Utils::Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Utils::Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Utils::Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Utils::Camera_Movement::RIGHT, deltaTime);
    // Use escape key for terminating the GLFW window
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
