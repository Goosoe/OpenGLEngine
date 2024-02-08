#include "TerrainModel.h"
#include "FastNoiseLite.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

// uncomment to disable assert()
// #define NDEBUG
//
#define assertm(exp, msg) assert(((void)msg, exp))

namespace Terrain
{
TerrainModel::TerrainModel(float length, int divPerSide)
{
    generateMesh(length, divPerSide);
    //setupTerrain();
}

void TerrainModel::setupTerrain()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //todo: make a vector/array with vertices, normal, texturecoords
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ModelLoader::Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
            &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (void*)0);
    //vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelLoader::Vertex), (void*)offsetof(ModelLoader::Vertex, normal));
    // vertex texture coords
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void TerrainModel::draw()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TerrainModel::generateMesh(float length, int divPerSide)
{
    assert(length > 0 && "Length must be > 0");
    assert(divPerSide > 0 && " must be > 0");
    const float polygonLength = length / divPerSide;
    constexpr float MULTIPLIER = 5.f;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    // vertices - 3 coords, 3 normals, 2 texpoints
    // TODO: normals and texture coods properly
    // Maybe make a data struct for each type of data we want to calculate and in the end, combine them all together in an array/vector

    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            glm::vec3 pos = glm::vec3(polygonLength * x,
                                      noise.GetNoise((float)x * MULTIPLIER, (float)y * MULTIPLIER),
                                      polygonLength * y);
            vertices.emplace_back(ModelLoader::Vertex{pos, glm::vec3(0.f)});
            // vertices.emplace_back(polygonLength * x); // / length); //x
            // vertices.emplace_back(noise.GetNoise((float)x * MULTIPLIER, (float)y * MULTIPLIER));  //y
            // vertices.emplace_back(polygonLength * y); // / length);  //z
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
    for(int i = 0; i < indices.size(); i += 3)
    {
        glm::vec3 v1 = vertices[indices[i]].position - vertices[indices[i + 1]].position;
        glm::vec3 v2 = vertices[indices[i]].position - vertices[indices[i + 2]].position;
        //now we have face normals!
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
        vertices[indices[i]].normal = normal;
        vertices[indices[i + 1]].normal = normal;
        vertices[indices[i + 2]].normal = normal;
    }
}
};
