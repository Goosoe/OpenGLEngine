#include "TerrainGenerator.h"
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
    TerrainData meshData;

public:
    TerrainModel(float length, int divPerSide);
    void draw();

private:
    void setupTerrain();
};
}
