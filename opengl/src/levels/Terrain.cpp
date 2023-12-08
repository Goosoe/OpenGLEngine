#include "Terrain.h"
#include "GLFW/glfw3.h"
#include "Entity.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float4.hpp"
#include "Camera.h"
#include "Commons.h"
#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>


//TODO: This function is repeated 
/**
 * iterates over shader vector and updates their common uniforms every frame
 */
void updateUniformsOfTerrainShaders(const std::vector<ShaderData>& shaders, const Camera& camera)
{
    for(size_t i = 0; i < shaders.size(); i++)
    { 
        GLuint program = shaders[i].program; 
        glUseProgram(program);
        Shader::setVec3(program, "cameraPos", camera.Position);
        glUseProgram(0);
    }
}

void runTerrainLevel(GLFWwindow* window)
{
    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(1, 1, 5));

    constexpr float ambientLight = .1f;
    constexpr float specularVal = 0.8f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 objColor (0.5f, 0.f, 0.f);

    //TODO: add resizable window feature
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH/ SCR_HEIGHT, 0.1f, 100.f);

    // setup window data
    setupWindowData(&camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

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
    std::vector<::ShaderData> shaders;
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
        //====

        models.emplace_back("./opengl/models/teapot.stl");
        Model& teapot = models.back();
        
        teapot.addEntity(lightShader.program, projection, glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(2.f));

       models.emplace_back("./opengl/models/grid.stl");
       Model& plane = models.back();
       
       //plane.addEntity(basicShader.program, projection, glm::vec3(1.f), glm::vec3(0.f));
       plane.addEntity(basicShader.program, projection, glm::vec3(1.f), glm::vec3(0.f), RotationData{90, glm::vec3(-1.f, 0.0f, 0.f)});
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

        updateUniformsOfTerrainShaders(shaders, camera);
        
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
