#include <vector>
#include <glad/glad.h>
namespace Terrain
{

    struct TerrainData
    {
        std::vector<float> vboData;
        std::vector<GLuint> indices;
    };
    /**
    * Generates a vector of vertices and normals of a plane mesh
    */
    void generateMesh(float length, int divPerSide, TerrainData& meshData);
};
