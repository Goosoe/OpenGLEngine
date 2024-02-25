#include "Patch.h"

#include <iostream>

//uncomment this if somehow we do not have the implementation done in another file before
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Patch
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
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_PATCHES, 0, vertices.size());
    glBindVertexArray(0);
}

/** MODEL **/
Patch::Patch(){}

void Patch::addEntity(GLuint shaderId, glm::mat4 projection, glm::vec3 scale, glm::vec3 location, RotationData rotation)
{
    entities.emplace_back(shaderId, projection, scale, location, rotation);
}

void Patch::drawEntities(glm::mat4& view)
{
    for(size_t i = 0; i < entities.size(); i++)
    {
        //todo: this is inneficient. using a program and deactivating it every loop
        GLuint currentShaderId = entities[i].shaderProgram; 
        glUseProgram(currentShaderId);
        entities[i].setViewUniform(view);
        for(unsigned int j = 0; j < meshes.size(); j++)
        {
            meshes[i].draw(currentShaderId);
        }
        glUseProgram(0);
    }
}

void Patch::unloadData()
{
    for(size_t i = 0; i < meshes.size(); i++)
    {
        glDeleteVertexArrays(1, &meshes[i].VAO);
        glDeleteBuffers(1, &meshes[i].VBO);
    }
}

unsigned int textureFromFile(const char *filename, const std::string &directory)
{
    std::string fullPath = std::string(filename);
    fullPath = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);
    if (!data)
    {
        std::cout << "Texture failed to load at path: " << fullPath << std::endl;
        stbi_image_free(data);
    }

    GLenum format;
    if (nrComponents == 1)
    {
        format = GL_RED;
    }
    else if (nrComponents == 3)
    {
        format = GL_RGB;
    }
    else //if (nrComponents == 4)
    {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}
};  //namespace Patch
