#include "Terrain.h"
#include "FastNoiseLite.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
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
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG
//
#define assertm(exp, msg) assert(((void)msg, exp))

/**
 * iterates over shader vector and updates their common uniforms
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

/**
* Procedurally generates a mesh with the given parameters
*/
void generateMesh(float length, int divPerSide, std::vector<ModelLoader::Vertex>& vertices, std::vector<GLuint>& indices)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;
    constexpr float MULTIPLIER = 5.f;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    // vertices - 3 coords, 3 normals, 2 texpoints
    // TODO: normals and texture coods properly

    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            glm::vec3 pos = glm::vec3(polygonLength * x,
                                      noise.GetNoise((float)x * MULTIPLIER, (float)y * MULTIPLIER),
                                      polygonLength * y);
            vertices.emplace_back(ModelLoader::Vertex{pos, glm::vec3(0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f)});
            //normals
        //    vboData.emplace_back(0.f);
        //    vboData.emplace_back(0.f);
        //    vboData.emplace_back(1.f);
        //    //textures
        //    vboData.emplace_back(0.f);
        //    vboData.emplace_back(0.f);
        }
    }

    //indices
    for(int y = 0; y < divPerSide; y++)
    {
        for(int x = 0; x < divPerSide; x++)
        {
            //verts
            indices.emplace_back((divPerSide + 1) * y + x);
            indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 1);
            indices.emplace_back((divPerSide + 1) * y + x + 1);

            indices.emplace_back((divPerSide + 1) * y + x + 1);
            indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 1);
            indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 2);
        }
   }
    // normals - always a multiple of 3
    for(size_t i = 0; i < indices.size(); i += 3)
    {
        glm::vec3 v1 = vertices[indices[i]].position - vertices[indices[i + 1]].position;
        glm::vec3 v2 = vertices[indices[i]].position - vertices[indices[i + 2]].position;
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
        vertices[indices[i]].normal = normal;
        vertices[indices[i + 1]].normal = normal;
        vertices[indices[i + 2]].normal = normal;
    }
}

void runTerrainLevel(GLFWwindow* window)
{
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 100;
    constexpr int TERRAIN_LENGTH = 100;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(1, 1, 5));

    constexpr float ambientLight = 0.4f;
    constexpr float specularVal = 0.1f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 objColor (0.f, 1.f, 0.f);

    //TODO: add resizable window feature
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH/ SCR_HEIGHT, 0.1f, 100.f);

    // setup window data
    setupWindowData(&camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

    glm::mat4 view(1.f);
    const glm::vec3 lightPos (5.f);

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float prevTime = (float) glfwGetTime();

    //setup data vectors
    std::vector<ModelLoader::Model> models;
    //for normal shaders that do not emit light
    std::vector<ShaderData> shaders;
    //for shaders that emit light
    // std::vector<ShaderData> illuminationShaders;

    {
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
        //Shader::setMVPUniforms(basicShader.program, modelMatrix, view, projection);
        glUseProgram(0);

        shaders.emplace_back(basicShader);
        //====
        std::vector<ModelLoader::Vertex> vertices;
        std::vector<GLuint> indices;
        generateMesh(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices, indices);
        ModelLoader::Model terrain;
        terrain.meshes.emplace_back(vertices, indices, std::vector<ModelLoader::Texture>());
        terrain.addEntity(basicShader.program, projection);
        models.emplace_back(terrain);

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
