#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "Entity.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

/**
 *  Assimp loaded models will be converted to a Model.
 *  Each Model holds information of the entities in the scene/level
 */
struct Vertex 
{
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
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    //  render data
    GLuint VAO, VBO, EBO;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    //Draws loaded meshes
    void drawLoaded(GLuint shaderProgram);
    //Draws handmade meshes 
    //void drawHandmade(GLuint shaderProgram);
private:

    void setupMesh();
};  

/**
 * A model will hold information about all the necessary data for it to be rendered in a scene. 
 * It will also hold information regarding entities of itself that are present in the scene
 */
class Model 
{
public:
    // model data
    std::vector<Mesh> meshes;

    std::vector<Texture> texturesLoaded;

    std::string directory;

    //Entities to render that use this model data
    std::vector<Entity> entities;

    // in the future if we want to hide entities from rendering
    //std::vector<Entity> hiddenEntities;

    //empty builder for creating our own models objects
    Model();

    Model(std::string path)
    {
        loadModel(path);
    }

    //todo: add entity with move semantics
    void addEntity(GLuint shaderProgram, glm::mat4 projection, glm::vec3 scale = glm::vec3(1.f),
                   glm::vec3 location = glm::vec3(0.f), RotationData rotationData = {0.f, glm::vec3(1.f)});

    /**
     * Use this to draw all entities registered using the drawLoaded function,
     * which draws using the loaded model standard naming conventions
     */
    void drawEntities(glm::mat4& view);

    /**
     * Use this to draw all entities registered using a custom drawFunction
     */
    void drawEntities(glm::mat4& view, void (*drawFunc)(GLuint, Mesh& mesh));

    /**
     * Call this to unload any allocated shader Buffers.
     * Recommended use on level/scene change
     */
    void unloadData();

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
unsigned int textureFromFile(const char *path, const std::string &directory);
