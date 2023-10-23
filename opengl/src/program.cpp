// Local headers
#include "program.h"
#include "GLFW/glfw3.h"
#include "utils/window.h"
#include "utils/shader.h"
#include "utils/TextureLoader.h"
#include <iostream>
#include <math.h>


void runProgram(GLFWwindow* window)
{
    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);


    // Scene setup

    //texture loading
    texture::loadTexture(GL_TEXTURE0, "./textures/wall.jpg");
    texture::loadTexture(GL_TEXTURE1, "./textures/banana.png");

    constexpr float vertices[] = {
    //  coords(3)          RGBA(4)                 textureCoord(2)
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top left 
    };  

    constexpr unsigned int indices[] = {  
        3, 1, 0,   // first triangle
        3, 2, 1    // second triangle
    };

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
     

    Utils::Shader defaultShader;
    
    defaultShader.makeBasicShader("./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");
    defaultShader.activate();

    //set the uniforms
    glUniform1i(glGetUniformLocation(defaultShader.get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(defaultShader.get(), "tex2"), 1);

    //int vColorLocation = glGetUniformLocation(defaultShader.get(), "tex");
    //glUniformSa(vColorLocation, colVal, .5f, .3f, 1.0f);

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        //float timeVal = glfwGetTime();
        //float colVal = abs(sin(timeVal));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }

    defaultShader.deactivate();
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
