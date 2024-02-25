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
namespace Patch
{
struct Vertex 
{
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
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
    std::vector<Texture> textures;

    //  render data
    GLuint VAO, VBO;

    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);

    //Draws loaded meshes
    //Draws handmade meshes 
    void draw(GLuint shaderProgram);
private:

    void setupMesh();
};  

/**
 * A model will hold information about all the necessary data for it to be rendered in a scene. 
 * It will also hold information regarding entities of itself that are present in the scene
 */
class Patch 
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
    Patch();

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
    //void drawEntities(glm::mat4& view, void (*drawFunc)(GLuint, Mesh& mesh));

    /**
     * Call this to unload any allocated shader Buffers.
     * Recommended use on level/scene change
     */
    void unloadData();
};

/**
 * Loads a texture from path and retuns the textureID
 */
unsigned int textureFromFile(const char *path, const std::string &directory);
}
