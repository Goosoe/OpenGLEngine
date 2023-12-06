#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include "Entity.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

/**
 *  Assimp loaded models will be converted to a Model.
 *  Each Model holds information of the entities in the scene/level
 */

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;

    //bone indexes which will influence this vertex
    //int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    //float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void draw(GLuint shaderProgram);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};  

class Model 
{
    private:
        // model data
        std::vector<Mesh> meshes;

        std::vector<Texture> texturesLoaded;

        std::string directory;

        //Entities to render that use this model data
        std::vector<Entity> entities;
        
        // in the future if we want to hide entities from rendering
        //std::vector<Entity> hiddenEntities;

    public:
        Model(std::string path)
        {
            loadModel(path);
        }

        //todo: add entity with move semantics
        void addEntity(GLuint shaderProgram, glm::mat4 projection, glm::vec3 scale = glm::vec3(1.f), glm::vec3 location = glm::vec3(0.f));

        void drawEntities(glm::mat4& view);
    private:

        void loadModel(std::string path);

        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        void loadMaterialTextures(std::vector<Texture>& textures, aiMaterial *mat, aiTextureType type, 
                std::string typeName);
};

/**
 * Loads a texture from path and retuns the textureID
 */
unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);
