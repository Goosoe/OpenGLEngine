#include "Terrain.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Camera.h"
#include "Commons.h"
#include "modelLoader/Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh/TerrainModel.h"


//TODO: This function is repeated 
/**
 * iterates over shader vector and updates their common uniforms every frame
 */
//void updateUniformsOfTerrainShaders(const std::vector<ShaderData>& shaders, const Camera& camera)
//{
//    for(size_t i = 0; i < shaders.size(); i++)
//    { 
//        GLuint program = shaders[i].program; 
//        glUseProgram(program);
//        Shader::setVec3(program, "cameraPos", camera.Position);
//        glUseProgram(0);
//    }
//}

void runTerrainLevel(GLFWwindow* window)
{
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 30;
    constexpr int TERRAIN_LENGTH = 10;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(1, 1, 5));

    constexpr float ambientLight = 0.7f;
    constexpr float specularVal = 0.2f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 objColor (1.f, 1.f, 1.f);

    //TODO: add resizable window feature
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH/ SCR_HEIGHT, 0.1f, 100.f);

    // setup window data
    setupWindowData(&camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

    glm::mat4 view(1.f);
    glm::mat4 modelMatrix = glm::mat4(1.f);
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationData.angle), rotationData.rotationAxis);
    //modelMatrix = glm::translate(modelMatrix, location);
    //modelMatrix = glm::scale(modelMatrix, scale);
    //normalMatrix = transpose(inverse(glm::mat3(modelMatrix)));
    const glm::vec3 lightPos (2.f);

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float prevTime = (float) glfwGetTime();

    //for normal shaders that do not emit light
    //std::vector<ShaderData> shaders;
    //for shaders that emit light
    // std::vector<ShaderData> illuminationShaders;

    //{
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

        //shaders.emplace_back(basicShader);
        //====
    //}
    //Length, subdivisions per side
    Terrain::TerrainModel terrain(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE);

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

        // updateUniformsOfTerrainShaders(shaders, camera);
        glUseProgram(basicShader.program);
        Shader::setMVPUniforms(basicShader.program, modelMatrix, view, projection);
        Shader::setVec3(basicShader.program, "cameraPos", camera.Position);
        // Shader::setViewUniform(basicShader.program, view);
        // Shader::setProjectionUniform(basicShader.program, projection);
        // Shader::setModelUniform(basicShader.program, modelMatrix);
        terrain.draw();
        glUseProgram(0);
        prevTime = currTime;
        
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}
