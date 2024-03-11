#include "ModelTesselation.h"
#include <iostream>


namespace ModelTesselation
{
/** MESH **/
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures) :
    vertices(vertices),
    textures(textures)
{
    setupMesh();
};

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    //todo: magic number
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glBindVertexArray(0);
}

void Mesh::draw(GLuint shaderId)
{
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        // set proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        //bind/activate existing texture unit
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
        //assign uniforms of current shader to texture unit
        Shader::setInt(shaderId, ("tex" + std::to_string(i)), i);
    }
    // glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_PATCHES, 0, vertices.size());
    glBindVertexArray(0);
}

/** MODEL **/
Model::Model(){}

void Model::addEntity(GLuint shaderId, glm::mat4 projection, glm::vec3 scale, glm::vec3 location, RotationData rotation)
{
    entities.emplace_back(shaderId, projection, scale, location, rotation);
}

void Model::drawEntities(glm::mat4& view)
{
    for(size_t i = 0; i < entities.size(); i++)
    {
        //todo: this is inneficient. using a program and deactivating it every loop
        GLuint currentShaderId = entities[i].shaderProgram; 
        glUseProgram(currentShaderId);
        entities[i].setViewUniform(view);
        for(unsigned int j = 0; j < meshes.size(); j++)
        {
            meshes[j].draw(currentShaderId);
        }
        glUseProgram(0);
    }
}

void Model::unloadData()
{
    for(size_t i = 0; i < meshes.size(); i++)
    {
        glDeleteVertexArrays(1, &meshes[i].VAO);
        glDeleteBuffers(1, &meshes[i].VBO);
    }
}
};  //namespace Patch
