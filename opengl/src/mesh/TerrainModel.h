#include <vector>
#include <glad/glad.h>

/**
*   Class for holding all the Terrain model data and functions to modify it
*/
namespace Terrain
{
class TerrainModel
{
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    std::vector<float> vboData;
    std::vector<GLuint> indices;

public:
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
