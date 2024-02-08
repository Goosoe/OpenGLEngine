#include <vector>
#include "modelLoader/Model.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

/**
*   Class for holding all the Terrain model data and functions to modify it
*/
namespace Terrain
{
class TerrainModel
{
public:
    //  render data
    unsigned int VAO, VBO, EBO;

    std::vector<ModelLoader::Vertex> vertices;
    //std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;

    TerrainModel(float length, int divPerSide);
    void draw();

private:
    /**
     * Initializes the necessary opengl buffers and fills them withe relevant information
     */
    void setupTerrain();
    /**
    * Generates a vector of vertices and normals of a plane mesh
    */
    void generateMesh(float length, int divPerSide);
};
}
