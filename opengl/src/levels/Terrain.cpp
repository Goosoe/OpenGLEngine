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

constexpr float NOISE_MULTIPLIER = 2.f;
constexpr float PEAKS_MULTIPLIER = 0.25f;
constexpr float SUB_PEAKS_MULTIPLIER = 8.f;
//todo: send this as uniform
constexpr float HEIGHT_MULTIPLIER = 2.f;
constexpr float UV_MULTIPLIER = 1.f;

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
        //todo: the height multiplier is no longer valid with the current implementation
        Shader::setFloat(program, "heightMultiplier", HEIGHT_MULTIPLIER);
        glUseProgram(0);
    }
}

/**
* Procedurally generates a mesh with the given parameters
*/
void generateMesh(const float length, const int divPerSide, std::vector<ModelLoader::Vertex>& vertices, std::vector<GLuint>& indices)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    const float uvCoord = length / divPerSide;
    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            glm::vec3 pos = glm::vec3(polygonLength * x,
                                      // (noise.GetNoise((float)x * NOISE_MULTIPLIER, (float)y * NOISE_MULTIPLIER) * HEIGHT_MULTIPLIER),
                                      //(noise.GetNoise((float)x * NOISE_MULTIPLIER, (float)y * NOISE_MULTIPLIER) * HEIGHT_MULTIPLIER) + (noise.GetNoise((float)x * PEAKS_MULTIPLIER, (float)y * PEAKS_MULTIPLIER)),
                                      (noise.GetNoise((float)x * NOISE_MULTIPLIER, (float)y * NOISE_MULTIPLIER) * HEIGHT_MULTIPLIER) + (noise.GetNoise((float)x * PEAKS_MULTIPLIER, (float)y * PEAKS_MULTIPLIER)) + (noise.GetNoise((float)x * SUB_PEAKS_MULTIPLIER, (float)y * SUB_PEAKS_MULTIPLIER)) / 5 ,
                                      polygonLength * y);
            vertices.emplace_back(ModelLoader::Vertex{
                pos, //position
                glm::vec3(0.f), //normal (to be set later)
                UV_MULTIPLIER * glm::vec2(x * uvCoord, y * uvCoord), // texCoords
                glm::vec3(0.f),
                glm::vec3(0.f)
            });
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
    //used for averaging the normals calculated
    std::vector<int> indicesRepeated(vertices.size(), 0);
    // normals - always a multiple of 3
    for(size_t i = 0; i < indices.size(); i += 3)
    {
        glm::vec3 v1 = vertices[indices[i]].position - vertices[indices[i + 1]].position;
        glm::vec3 v2 = vertices[indices[i]].position - vertices[indices[i + 2]].position;
        glm::vec3 normal = glm::cross(v1, v2);
        //sum all the normals acting on a vertex
        vertices[indices[i]].normal += normal;
        vertices[indices[i + 1]].normal += normal;
        vertices[indices[i + 2]].normal += normal;
        //store how many times the vertex normal was summed
        indicesRepeated[indices[i]]++;
        indicesRepeated[indices[i + 1]]++;
        indicesRepeated[indices[i + 2]]++;
    }
    //average the summed normals and normalize them 
    for(size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].normal = glm::normalize(vertices[i].normal / glm::vec3(indicesRepeated[i]));
    }
}

void runTerrainLevel(GLFWwindow* window)
{
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 300;
    constexpr int TERRAIN_LENGTH = 50;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(TERRAIN_LENGTH / 2, 3, TERRAIN_LENGTH / 2), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);

    constexpr float ambientLight = 0.4f;
    constexpr float specularVal = 0.1f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 objColor (1.f, 1.f, 1.f);

    //TODO: add resizable window feature
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH/ SCR_HEIGHT, 0.1f, 100.f);

    // setup window data
    setupWindowData(&camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

    glm::mat4 view(1.f);
    const glm::vec3 lightPos (TERRAIN_LENGTH / 2, TERRAIN_LENGTH / 2, 5);

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //todo: create a first time mouse_callback to calibrate and then re-set the cursor callback as the default one
    glfwSetCursorPosCallback(window, mouseCallbackAdjustCursor);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float prevTime = (float) glfwGetTime();

    //setup data vectors
    // std::vector<ModelLoader::Model> models;
    //for normal shaders that do not emit light
    ModelLoader::Model terrain;
    std::vector<ShaderData> shaders;
    //for shaders that emit light
    // std::vector<ShaderData> illuminationShaders;

    {
        //terrain shader
        ShaderData basicShader;
        basicShader.program = Shader::createProgram();
        Shader::makeBasicShader(basicShader, "./opengl/shaders/TerrainV.glsl", "./opengl/shaders/TerrainF.glsl");

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
        terrain.meshes.emplace_back(vertices, indices, std::vector<ModelLoader::Texture>());
        terrain.addEntity(basicShader.program, projection);
        terrain.meshes[0].textures.emplace_back(
            ModelLoader::Texture{
                ModelLoader::textureFromFile("grass.jpg", "./textures"),
                "",
                "",
            }
        );
        terrain.meshes[0].textures.emplace_back(
            ModelLoader::Texture{
                ModelLoader::textureFromFile("rock.jpg", "./textures"),
                "",
                ""
            }
        );
        terrain.meshes[0].textures.emplace_back(
            ModelLoader::Texture{
                ModelLoader::textureFromFile("snow.jpg", "./textures"),
                "",
                ""
            }
        );

        //models.emplace_back(terrain);
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

        //todo: remove this. This is for test reasons
        //Shader::setFloat(shaders[0].program, "elapsedTime", currTime);
        updateUniformsOfTerrainShaders(shaders, camera);

        glUseProgram(shaders[0].program);
        Shader::setViewUniform(shaders[0].program, view);
        for(unsigned int i = 0; i < terrain.meshes.size(); i++)
        {
            terrain.meshes[i].drawHandmade(shaders[0].program);
        }
        glUseProgram(0);

        //use case when having loaded models
       // for(size_t i = 0; i < models.size(); i++)
       // { 
       //     //todo: texture is being put in mesh and is doing unnecessary bs (Models.cpp l.64)
       //     models[i].drawEntities(view);
       // }
        prevTime = currTime;
        
        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}
