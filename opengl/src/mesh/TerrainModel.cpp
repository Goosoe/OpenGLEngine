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
    setupTerrain();
}

void TerrainModel::setupTerrain()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(float), &vboData[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
            &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // vertex normals
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(3 * sizeof(float)));
    // // vertex texture coords
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(5 * sizeof(float)));
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
    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            vboData.emplace_back(polygonLength * x); // / length); //x
            vboData.emplace_back(noise.GetNoise((float)x * MULTIPLIER, (float)y * MULTIPLIER));  //y
            vboData.emplace_back(polygonLength * y); // / length);  //z
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
}
};
