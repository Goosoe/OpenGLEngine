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
void updateRenderShader(const GLuint shader, const Camera& camera)
{
    glUseProgram(shader);
    Shader::setVec3(shader, "cameraPos", camera.position);
    glUseProgram(0);
}

void updateShadowMapUniforms(const GLuint shadowMapShader, const glm::mat4& entityModel)
{
    glUseProgram(shadowMapShader);
    Shader::setMat4(shadowMapShader, "model", entityModel);
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
    //Terrain settings
    constexpr int TERRAIN_POLYGONS_PER_SIDE = 1;
    constexpr int TERRAIN_LENGTH = 50;
    constexpr float NEAR_PLANE = 1.0f;
    constexpr float FAR_PLANE = 50.f;

    // GL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // default clearing colour 
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    //required variables/ consts

    Camera camera(glm::vec3(TERRAIN_LENGTH / 4, 10, TERRAIN_LENGTH / 4), glm::vec3(0.0f, 1.0f, 0.0f), 0, -10);
    //TODO: Make api for framebuffer creation
    unsigned int depthMapFBO;
    unsigned int depthMap;
    constexpr int SHADOW_WIDTH = 1024;
    constexpr int SHADOW_HEIGHT = 1024;
    //Create framebuffer for depth map
    {
        glGenFramebuffers(1, &depthMapFBO);  

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);  

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    }

    constexpr float ambientLight = 0.4f;
    constexpr float specularVal = 0.1f;

    //Scene data
    const glm::vec3 lightColor (1.f, 1.f, 1.f);
    const glm::vec3 terrainColor (0.f, 1.f, 0.f);
    const glm::vec3 cubeColor (1.f, 0.f, 0.f);

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
    Model terrain;
    Model cubeModel;
    //Final render shaders
    ShaderData terrainShader;
    ShaderData cubeShader;
    //Depth map shader pass
    ShaderData shadowMapPassShader;
    //Depth map display shader pass
    ShaderData shadowMapRenderShader;

    //required for rendering the shadowMapRenderShader. As for running a shader properly, it requires a VAO, even if not used
    GLuint emptyVAO; 
    glGenVertexArrays(1, &emptyVAO); 
    glm::mat4 lightSpaceMatrix;

    //Prepares the shadow map pass program
    {
        shadowMapPassShader.program = Shader::createProgram();
        Shader::makeBasicShader(shadowMapPassShader, "./opengl/shaders/shadow/ShadowmapPassV.glsl", "./opengl/shaders/shadow/ShadowmapPassF.glsl");

        glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, NEAR_PLANE, FAR_PLANE);
        glm::mat4 lightView = glm::lookAt(lightPos,
                                          glm::vec3( TERRAIN_LENGTH / 2 , 0.0f,  TERRAIN_LENGTH / 2), 
                                          glm::vec3( 0.0f, 1.0f,  0.0f)); 
        lightSpaceMatrix = lightProjection * lightView;

        glUseProgram(shadowMapPassShader.program);
        Shader::setMat4(shadowMapPassShader.program, "lightSpaceMatrix", lightSpaceMatrix);
        glUseProgram(0);
    }

    //Prepares the render terrain shader program
    {
        terrainShader.program = Shader::createProgram();
        Shader::makeBasicShader(terrainShader, "./opengl/shaders/shadow/ShadowModelV.glsl", "./opengl/shaders/shadow/ShadowModelF.glsl");

        glUseProgram(terrainShader.program);
        Shader::setVec3(terrainShader.program, "lightPos", lightPos);
        Shader::setVec3(terrainShader.program, "lightColor", lightColor);
        Shader::setVec3(terrainShader.program, "objColor", terrainColor);
        Shader::setMat4(terrainShader.program, "lightSpace", lightSpaceMatrix);
        Shader::setFloat(terrainShader.program, "ambientVal", ambientLight);
        Shader::setFloat(terrainShader.program, "specularVal", specularVal);
        glUseProgram(0);
    }

    //Prepares the render cube shader program
    {

        cubeShader.program = Shader::createProgram();
        Shader::makeBasicShader(cubeShader, "./opengl/shaders/BasicV.glsl", "./opengl/shaders/BasicF.glsl");

        glUseProgram(cubeShader.program);
        Shader::setVec3(cubeShader.program, "lightPos", lightPos);
        Shader::setVec3(cubeShader.program, "lightColor", lightColor);
        Shader::setVec3(cubeShader.program, "objColor", cubeColor);
        Shader::setMat4(terrainShader.program, "lightSpace", lightSpaceMatrix);
        Shader::setFloat(cubeShader.program, "ambientVal", ambientLight);
        Shader::setFloat(cubeShader.program, "specularVal", specularVal);
        glUseProgram(0);
    }

    //Prepares the shader to show the result of the render pass (DEBUG ONLY)
    // {
    //     shadowMapRenderShader.program = Shader::createProgram();
    //     Shader::makeBasicShader(shadowMapRenderShader, "./opengl/shaders/shadow/ShadowmapDepthShowV.glsl", "./opengl/shaders/shadow/ShadowmapDepthShowF.glsl");
    //
    //     glUseProgram(shadowMapRenderShader.program);
    //     glBindTexture(GL_TEXTURE_2D, depthMap);
    //     glUseProgram(0);
    // }

    //Sets the terrain model
    {
        //====
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        Shadow::generateSimpleMesh(TERRAIN_LENGTH, TERRAIN_POLYGONS_PER_SIDE, vertices, indices);
        terrain.meshes.emplace_back(vertices, indices, std::vector<Texture>());
        terrain.addEntity(terrainShader.program, projection);
    }

    //Sets the cube model
    {
        //====
        cubeModel.meshes.emplace_back(ModelData::cube, std::vector<Texture>());
        constexpr float scale = 10.f;
        cubeModel.addEntity(cubeShader.program, projection, glm::vec3(scale), glm::vec3(TERRAIN_LENGTH / 2, scale * .5f, TERRAIN_LENGTH / 2));
    }

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        float currTime = (float) glfwGetTime();
        float deltaTime = currTime - prevTime;

        // update view matrix with updated camera datashadow.cpp
        view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        // Draw your scene here
        //Depth map pass
        { 
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            for(int i = 0; i < cubeModel.entities.size(); i++)
            {
                Shadow::updateShadowMapUniforms(shadowMapPassShader.program, cubeModel.entities[i].getModelMat());
                glUseProgram(shadowMapPassShader.program);
                for(unsigned int j = 0; j < cubeModel.meshes.size(); j++)
                {
                    Shadow::drawCubeEntity(shadowMapPassShader.program, cubeModel.meshes[j]);
                }
                glUseProgram(0);
            }

            for(int i = 0; i < terrain.entities.size(); i++)
            {
                Shadow::updateShadowMapUniforms(shadowMapPassShader.program, terrain.entities[i].getModelMat());
                glUseProgram(shadowMapPassShader.program);
                for(unsigned int j = 0; j < terrain.meshes.size(); j++)
                {
                    Shadow::drawTerrainEntity(shadowMapPassShader.program, terrain.meshes[j]);
                }
                glUseProgram(0);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(0);
        }


        //TODO: Make a toggle button to switch between passes
        //Depth debug Render Pass
       // {
       //      glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
       //      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //      glDisable(GL_DEPTH_TEST);
       //      glClear(GL_COLOR_BUFFER_BIT);
       //      glBindVertexArray(emptyVAO);
       //      glUseProgram(shadowMapRenderShader.program);
       //
       //      glActiveTexture(GL_TEXTURE0);
       //      glBindTexture(GL_TEXTURE_2D, depthMap);
       //
       //      glDrawArrays(GL_TRIANGLES, 0, 3);
       //      glUseProgram(0);
       //      glBindVertexArray(0);
       //      glEnable(GL_DEPTH_TEST);
       // }

        //Render Pass
       {
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shadow::updateRenderShader(terrainShader.program, camera);
            Shadow::updateRenderShader(cubeShader.program, camera);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            terrain.drawEntities(view, &Shadow::drawTerrainEntity);
            cubeModel.drawEntities(view, &Shadow::drawCubeEntity);
            glActiveTexture(0);
            glBindTexture(GL_TEXTURE_2D, 0);
       }
        prevTime = currTime;

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, deltaTime);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}
