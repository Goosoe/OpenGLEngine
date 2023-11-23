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


namespace modelLoader
{

    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
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

            void Draw(Utils::Shader &shader);
        private:
            //  render data
            unsigned int VAO, VBO, EBO;

            void setupMesh();
    };  

    
    class Model 
    {
        public:
            Model(std::string path)
            {
                loadModel(path);
            }

            void Draw(Utils::Shader &shader);	

        private:
            // model data
            std::vector<Mesh> meshes;
            std::vector<Texture> texturesLoaded;

            std::string directory;

            void loadModel(std::string path);

            void processNode(aiNode *node, const aiScene *scene);

            Mesh processMesh(aiMesh *mesh, const aiScene *scene);

            std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                    std::string typeName);
    };

    
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

}; 
