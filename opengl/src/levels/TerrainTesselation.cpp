#include "TerrainTesselation.h"
#include "FastNoiseLite.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Camera.h"
#include "Commons.h"
#include "Patch.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG
//
#define assertm(exp, msg) assert(((void)msg, exp))

//// Properties for terrain height generation
// constexpr int OCTAVES = 10;
// constexpr float LACUNARITY = 2.0;
// constexpr float GAIN = .5;
//

constexpr float UV_MULTIPLIER = 1.f;

/**
 * iterates over shader vector and updates their common uniforms
 */
void updateTerrainShader(const ShaderData& shader, const Camera& camera)
{
       const GLuint program = shader.program;
       glUseProgram(program);
       Shader::setVec3(program, "cameraPos", camera.Position);
       glUseProgram(0);
}

// void generateNoiseHeight(FastNoiseLite& noise, glm::vec3& pos)
// {
//     //calculate height
//     float amplitude = 2.;
//     float frequency = 1.;
//     // Loop of octaves
//     for (int i = 0; i < OCTAVES; i++) {
//         pos.y += amplitude * noise.GetNoise(frequency * pos.x, frequency * pos.y);
//         frequency *= LACUNARITY;
//         amplitude *= GAIN;
//     }
// }
/**
* Procedurally generates a mesh without taking into consideration any LOD, with the given parameters
*/
void generatePatch(const float length, const int divPerSide, std::vector<Patch::Vertex>& vertices)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;
    // const float uvCoord = length / divPerSide;


    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    //todo: normals not being used
    // const float uvCoord = length / divPerSide;
    for(int y = 0; y < divPerSide; y++)
    {
        for(int x = 0; x < divPerSide; x++)
        {
            //coords
            ::glm::vec3 pos1 = glm::vec3(polygonLength * x,    //x
                                        0,
                                        polygonLength * y);   //z
            // generateNoiseHeight(noise, pos1);

            // std::cout << pos1.x << "," << pos1.y << "," << pos1.z << "\n";

            ::glm::vec3 pos2 = glm::vec3(polygonLength * x,    //x
                                        0,
                                        polygonLength * y + polygonLength);   //z
            // generateNoiseHeight(noise, pos2);

            // std::cout << pos2.x << "," << pos2.y << "," << pos2.z << "\n";
            ::glm::vec3 pos3 = glm::vec3(polygonLength * x + polygonLength,    //x
                                        0,
                                        polygonLength * y);   //z
            // generateNoiseHeight(noise, pos3);

            // std::cout << pos3.x << "," << pos3.y << "," << pos3.z << "\n";

            ::glm::vec3 pos4 = glm::vec3(polygonLength * x + polygonLength,    //x
                                        0,
                                        polygonLength * y + polygonLength);   //z

            // generateNoiseHeight(noise, pos4);
            // std::cout << pos4.x << "," << pos4.y << "," << pos4.z << "\n";

            vertices.emplace_back(Patch::Vertex{
                pos1, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2(x / (float)polygonLength , y / (float)polygonLength), // texCoords
            });
            vertices.emplace_back(Patch::Vertex{
                pos2, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2(x / (float)polygonLength, (y + 1) / (float)polygonLength), // texCoords
            });
            vertices.emplace_back(Patch::Vertex{
                pos3, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2((x + 1) / (float)polygonLength, y / (float)polygonLength), // texCoords
            });
            vertices.emplace_back(Patch::Vertex{
                pos4, //position
                glm::vec3(0.f, 1.f, 0.f),   //normal
                glm::vec2((x + 1) / (float)polygonLength, (y + 1) / (float)polygonLength), // texCoords
            });
        }
    }

    //indices
   //  for(int y = 0; y < divPerSide; y++)
   //  {
   //      for(int x = 0; x < divPerSide; x++)
   //      {
   //          //verts
   //          indices.emplace_back((divPerSide + 1) * y + x);
   //          indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 1);
   //          indices.emplace_back((divPerSide + 1) * y + x + 1);
   //
   //          indices.emplace_back((divPerSide + 1) * y + x + 1);
   //          indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 1);
   //          indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 2);
   //      }
   // }
    //used for averaging the normals calculated
    //std::vector<int> indicesRepeated(vertices.size(), 0);
    // normals - always a multiple of 3
   // for(size_t i = 0; i < indices.size(); i += 3)
   // {
   //     glm::vec3 v1 = vertices[indices[i]].position - vertices[indices[i + 1]].position;
   //     glm::vec3 v2 = vertices[indices[i]].position - vertices[indices[i + 2]].position;
   //     glm::vec3 normal = glm::cross(v1, v2);
   //     //sum all the normals acting on a vertex
   //     vertices[indices[i]].normal += normal;
   //     vertices[indices[i + 1]].normal += normal;
   //     vertices[indices[i + 2]].normal += normal;
   //     //store how many times the vertex normal was summed
   //     indicesRepeated[indices[i]]++;
   //     indicesRepeated[indices[i + 1]]++;
   //     indicesRepeated[indices[i + 2]]++;
   // }
   // //average the summed normals and normalize them 
   // for(size_t i = 0; i < vertices.size(); i++)
   // {
   //     vertices[i].normal = glm::normalize(vertices[i].normal / glm::vec3(indicesRepeated[i]));
   // }
}

void runTerrainTesselationLevel(GLFWwindow* window)
{
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 1;
    constexpr int TERRAIN_LENGTH = 10;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    // Camera camera(glm::vec3(TERRAIN_LENGTH / 2, 3, TERRAIN_LENGTH / 2), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);
    Camera camera(glm::vec3(0, 3, 0), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);

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

    //setup data vectors
    // std::vector<Model> models;
    Patch::Patch terrain;
    ShaderData terrainShader;
    // std::vector<ShaderData> shaders;

    //Sets the terrain model
    {
        terrainShader.program = Shader::createProgram();
        // Shader::makeBasicShader(terrainShader, "./opengl/shaders/TerrainTesselationV.glsl", "./opengl/shaders/TerrainTesselationF.glsl");
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
        std::vector<Patch::Vertex> vertices;
        generatePatch(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices);
        terrain.meshes.emplace_back(vertices, std::vector<Patch::Texture>());
        terrain.addEntity(terrainShader.program, projection);
        terrain.meshes[0].textures.emplace_back(
            Patch::Texture{
                textureFromFile("heightmapSample.png", "./textures"),
                "",
                "",
            }
        );
        terrain.meshes[0].textures.emplace_back(
            Patch::Texture{
                textureFromFile("grass.jpg", "./textures"),
                "",
                "",
            }
        );
        terrain.meshes[0].textures.emplace_back(
            Patch::Texture{
                textureFromFile("rock.jpg", "./textures"),
                "",
                ""
            }
        );
        terrain.meshes[0].textures.emplace_back(
            Patch::Texture{
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

        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here

        // update view matrix with updated camera data
        view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

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
