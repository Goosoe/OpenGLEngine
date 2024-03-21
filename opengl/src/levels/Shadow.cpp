#include "Shadow.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Camera.h"
#include "Commons.h"
#include "Model.h"
#include "modelData/Cube.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <iostream>

// uncomment to disable assert()
// #define NDEBUG
//
namespace Shadow
{
//#define assertm(exp, msg) assert(((void)msg, exp))


/**
 * Use this function to draw the custom built terrain model entity
 */
void drawCubeEntity(GLuint shaderId, Mesh& mesh)
{
    // draw mesh 
    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
    glBindVertexArray(0);
}
void drawTerrainEntity(GLuint shaderId, Mesh& mesh)
{
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
        Shader::setVec3(program, "cameraPos", camera.position);
        glUseProgram(0);
}

/**
* Procedurally generates a mesh of given parameters
*/
void generateSimpleMesh(const float length, const int divPerSide, std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;

    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            glm::vec3 pos = glm::vec3(polygonLength * x,    //x
                                      0,
                                      polygonLength * y);   //z

            vertices.emplace_back(Vertex{
                pos, //position
                glm::vec3(0.f), //normal (to be set later)
                glm::vec3(0.f), 
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

}; //Shadow namespace

void runShadowLevel(GLFWwindow* window)
{
    //TODO: change variable names
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 1;
    constexpr int TERRAIN_LENGTH = 50;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(0.f), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);
    // Camera camera(glm::vec3(TERRAIN_LENGTH / 2, 3, TERRAIN_LENGTH / 2), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);

    constexpr float ambientLight = 0.4f;
    constexpr float specularVal = 0.1f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 objColor (0.f, 1.f, 0.f);

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
    Model cubeModel;
    ShaderData terrainShader;
    // std::vector<ShaderData> shaders;

    //Prepares the basic shader program
    {
        terrainShader.program = Shader::createProgram();
        Shader::makeBasicShader(terrainShader, "./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");

        glUseProgram(terrainShader.program);
        Shader::setVec3(terrainShader.program, "lightPos", lightPos);
        Shader::setVec3(terrainShader.program, "lightColor", lightColor);
        Shader::setVec3(terrainShader.program, "objColor", objColor);
        Shader::setFloat(terrainShader.program, "ambientVal", ambientLight);
        Shader::setFloat(terrainShader.program, "specularVal", specularVal);
        glUseProgram(0);
    }

    //Sets the terrain model
    {
        //====
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        Shadow::generateSimpleMesh(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices, indices);
        terrain.meshes.emplace_back(vertices, indices, std::vector<Texture>());
        terrain.addEntity(terrainShader.program, projection);
        //does nothing
        //glm::vec2 texSize;
        // terrain.meshes[0].textures.emplace_back(
        //     Texture{
        //         textureFromFile("grass.jpg", "./textures", texSize),
        //         "",
        //         "",
        //     }
        // );
        // terrain.meshes[0].textures.emplace_back(
        //     Texture{
        //         textureFromFile("rock.jpg", "./textures", texSize),
        //         "",
        //         ""
        //     }
        // );
        // terrain.meshes[0].textures.emplace_back(
        //     Texture{
        //         textureFromFile("snow.jpg", "./textures", texSize),
        //         "",
        //         ""
        //     }
        // );
    }
    //Sets the cube model
    {
        //====
        // Shadow::generateSimpleMesh(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices, indices);
        cubeModel.meshes.emplace_back(ModelData::cube, std::vector<Texture>());
        cubeModel.addEntity(terrainShader.program, projection, glm::vec3(5.f));
        //does nothing
        //glm::vec2 texSize;
        // terrain.meshes[0].textures.emplace_back(
        //     Texture{
        //         textureFromFile("grass.jpg", "./textures", texSize),
        //         "",
        //         "",
        //     }
        // );
        // terrain.meshes[0].textures.emplace_back(
        //     Texture{
        //         textureFromFile("rock.jpg", "./textures", texSize),
        //         "",
        //         ""
        //     }
        // );
        // terrain.meshes[0].textures.emplace_back(
        //     Texture{
        //         textureFromFile("snow.jpg", "./textures", texSize),
        //         "",
        //         ""
        //     }
        // );
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
        view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        Shadow::updateTerrainShader(terrainShader, camera);
        terrain.drawEntities(view, &Shadow::drawTerrainEntity);
        cubeModel.drawEntities(view, &Shadow::drawCubeEntity);

        prevTime = currTime;

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}
