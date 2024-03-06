#include "TerrainTesselation.h"
#include "FastNoiseLite.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include "Camera.h"
#include "Commons.h"
#include "ModelTesselation.h"
#include "Shader.h"
#include <cassert>
#include <iostream>

// uncomment to disable assert()
// #define NDEBUG

#define assertm(exp, msg) assert(((void)msg, exp))

/**
 * iterates over shader vector and updates their common uniforms
 */
void updateTerrainShader(const ShaderData& shader, const Camera& camera)
{
       const GLuint program = shader.program;
       glUseProgram(program);
       Shader::setVec3(program, "cameraPos", camera.position);
       glUseProgram(0);
}

/**
* Procedurally generates a mesh without taking into consideration any LOD, with the given parameters
*/
void generatePatch(const float length, const int divPerSide, std::vector<ModelTesselation::Vertex>& vertices)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;


    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    //todo: normals not being used
    // const float uvCoord = length / divPerSide;
    for(int y = 0; y < divPerSide; y++)
    {
        for(int x = 0; x < divPerSide; x++)
        {
            //coords
            glm::vec3 pos1 = glm::vec3(polygonLength * x,    //x
                                        0,
                                        polygonLength * y);   //z

            glm::vec3 pos2 = glm::vec3(polygonLength * x,    //x
                                        0,
                                        polygonLength * y + polygonLength);   //z
            glm::vec3 pos3 = glm::vec3(polygonLength * x + polygonLength,    //x
                                        0,
                                        polygonLength * y);   //z


            glm::vec3 pos4 = glm::vec3(polygonLength * x + polygonLength,    //x
                                        0,
                                        polygonLength * y + polygonLength);   //z

            vertices.emplace_back(ModelTesselation::Vertex{
                pos1, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2((float)(x) / divPerSide, (float)(y) / divPerSide), // texCoords
            });
            vertices.emplace_back(ModelTesselation::Vertex{
                pos2, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2((float)(x + 1) / divPerSide, (float)(y) / divPerSide), // texCoords
            });
            vertices.emplace_back(ModelTesselation::Vertex{
                pos3, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2((float)(x) / divPerSide, (float)(y + 1) / divPerSide), // texCoords
            });
            vertices.emplace_back(ModelTesselation::Vertex{
                pos4, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2((float)(x + 1) / divPerSide , (float)(y + 1) / divPerSide), // texCoords
            });
        }
    }
}

void runTerrainTesselationLevel(GLFWwindow* window)
{
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 50;
    constexpr int TERRAIN_LENGTH = 150;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(TERRAIN_LENGTH / 2, 8, TERRAIN_LENGTH / 2), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);
    // Camera camera(glm::vec3(TERRAIN_LENGTH / 4, 8, TERRAIN_LENGTH / 4), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);
    // Camera camera(glm::vec3(100.f, 8, 100.f), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);
    // Camera camera(glm::vec3(0, 3, 0), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);

    constexpr float ambientLight = 0.4f;
    constexpr float specularVal = 0.1f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 objColor (1.f, 1.f, 1.f);

    //TODO: add resizable window feature
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.f);

    // setup window data
    setupWindowData(&camera, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);

    glm::mat4 view(1.f);
    const glm::vec3 lightPos (TERRAIN_LENGTH / 4, TERRAIN_LENGTH / 4, 3);

    //TODO: set framebuffersize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouseCallbackAdjustCursor);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float prevTime = (float) glfwGetTime();

    ModelTesselation::Model terrain;
    ShaderData terrainShader;

    //Sets the terrain model
    {
        terrainShader.program = Shader::createProgram();
        Shader::attach(terrainShader, "./opengl/shaders/TerrainTV.glsl", Shader::Vertex);
        Shader::attach(terrainShader, "./opengl/shaders/TerrainTF.glsl", Shader::Fragment);
        Shader::attach(terrainShader, "./opengl/shaders/TerrainTTC.glsl", Shader::TessControl);
        Shader::attach(terrainShader, "./opengl/shaders/TerrainTTE.glsl", Shader::TessEval);
        Shader::link(terrainShader);

        glUseProgram(terrainShader.program);
        Shader::setVec3(terrainShader.program, "lightPos", lightPos);
        Shader::setVec3(terrainShader.program, "lightColor", lightColor);
        Shader::setVec3(terrainShader.program, "objColor", objColor);
        Shader::setFloat(terrainShader.program, "ambientVal", ambientLight);
        Shader::setFloat(terrainShader.program, "specularVal", specularVal);
        glUseProgram(0);

        //====
        std::vector<ModelTesselation::Vertex> vertices;
        generatePatch(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices);
        terrain.meshes.emplace_back(vertices, std::vector<ModelTesselation::Texture>());
        terrain.addEntity(terrainShader.program, projection);
        terrain.meshes[0].textures.emplace_back(
            ModelTesselation::Texture{
                textureFromFile("iceland_heightmap.png", "./textures"),
                "",
                "",
            }
        );
        terrain.meshes[0].textures.emplace_back(
            ModelTesselation::Texture{
                textureFromFile("grass.jpg", "./textures"),
                "",
                "",
            }
        );
        terrain.meshes[0].textures.emplace_back(
            ModelTesselation::Texture{
                textureFromFile("rock.jpg", "./textures"),
                "",
                ""
            }
        );
        terrain.meshes[0].textures.emplace_back(
            ModelTesselation::Texture{
                textureFromFile("snow.jpg", "./textures"),
                "",
                ""
            }
        );
    }

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        float currTime = (float) glfwGetTime();
        float deltaTime = currTime - prevTime;
        //std::cout << camera.position.x << " " << camera.position.y << " " << camera.position.z << "\n";

        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here

        // update view matrix with updated camera data
        view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        updateTerrainShader(terrainShader, camera);

        terrain.drawEntities(view);

        prevTime = currTime;

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}
