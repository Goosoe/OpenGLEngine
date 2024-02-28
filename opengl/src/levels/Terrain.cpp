#include "Terrain.h"
#include "FastNoiseLite.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Camera.h"
#include "Commons.h"
#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

// uncomment to disable assert()
// #define NDEBUG
//
namespace Terrain
{
#define assertm(exp, msg) assert(((void)msg, exp))

//// Properties for terrain height generation
constexpr int OCTAVES = 10;
constexpr float LACUNARITY = 2.0;
constexpr float GAIN = .5;
//

constexpr float UV_MULTIPLIER = 1.f;

/**
 * Use this function to draw the custom built terrain model entity
 */
void drawTerrainEntity(GLuint shaderId, Mesh& mesh)
{
    for(unsigned int i = 0; i < mesh.textures.size(); i++)
    {
        // set proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        //bind/activate existing texture unit
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
        //assign uniforms of current shader to texture unit
        Shader::setInt(shaderId, ("tex" + std::to_string(i)), i);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

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

/**
* Procedurally generates a mesh without taking into consideration any LOD, with the given parameters
*/
void generateSimpleMesh(const float length, const int divPerSide, std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            glm::vec3 pos = glm::vec3(polygonLength * x,    //x
                                      0,
                                      polygonLength * y);   //z
            //calculate height
            float amplitude = 2.;
            float frequency = 1.;
            // Loop of octaves
            for (int i = 0; i < OCTAVES; i++) {
                pos.y += amplitude * noise.GetNoise(frequency * x, frequency * y);
                frequency *= LACUNARITY;
                amplitude *= GAIN;
            }

            vertices.emplace_back(Vertex{
                pos, //position
                glm::vec3(0.f), //normal (to be set later)
                UV_MULTIPLIER * glm::vec2(x * polygonLength, y * polygonLength), // texCoords
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

}; //Terrain namespace
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
    Model terrain;
    ShaderData terrainShader;
    // std::vector<ShaderData> shaders;

    //Sets the terrain model
    {
        terrainShader.program = Shader::createProgram();
        Shader::makeBasicShader(terrainShader, "./opengl/shaders/TerrainV.glsl", "./opengl/shaders/TerrainF.glsl");

        glUseProgram(terrainShader.program);
        Shader::setVec3(terrainShader.program, "lightPos", lightPos);
        Shader::setVec3(terrainShader.program, "lightColor", lightColor);
        Shader::setVec3(terrainShader.program, "objColor", objColor);
        Shader::setFloat(terrainShader.program, "ambientVal", ambientLight);
        Shader::setFloat(terrainShader.program, "specularVal", specularVal);
        glUseProgram(0);

        //====
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        Terrain::generateSimpleMesh(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices, indices);
        terrain.meshes.emplace_back(vertices, indices, std::vector<Texture>());
        terrain.addEntity(terrainShader.program, projection);
        terrain.meshes[0].textures.emplace_back(
            Texture{
                textureFromFile("grass.jpg", "./textures"),
                "",
                "",
            }
        );
        terrain.meshes[0].textures.emplace_back(
            Texture{
                textureFromFile("rock.jpg", "./textures"),
                "",
                ""
            }
        );
        terrain.meshes[0].textures.emplace_back(
            Texture{
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

        Terrain::updateTerrainShader(terrainShader, camera);
        terrain.drawEntities(view, &Terrain::drawTerrainEntity);

        prevTime = currTime;

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}
