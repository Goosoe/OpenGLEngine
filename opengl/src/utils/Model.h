#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "Shader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


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

        void draw(Shader &shader);
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

      //  Shader 

    public:
        Model(std::string path)
        {
            loadModel(path);
        }

        void draw(Shader &shader);	
    private:

        void loadModel(std::string path);

        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                std::string typeName);
};

/**
 * Loads a texture from path and retuns the textureID
 */
unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);

/**
 * Loads a texture from path to the selected textureUnit.
 * It checks if it s a jpeg or png and does the necessary modifications
 */
//void loadTexture(const GLenum textureUnit, const std::string path);
