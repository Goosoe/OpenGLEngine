#include "TerrainGenerator.h"
#include <iostream>

namespace Terrain
{
void generateMesh(float length, int divPerSide, TerrainData& meshData)
{
    float polygonLength = length / divPerSide;
    // vertices - 3 coords, 3 normals, 2 texpoints
    // TODO: texture coods properly
    for(int y = 0; y <= divPerSide; y++)
    {
        for(int x = 0; x <= divPerSide; x++)
        {
            //coords
            meshData.vboData.emplace_back(polygonLength * x); // / length); //x
            meshData.vboData.emplace_back(0.f);  //y
            meshData.vboData.emplace_back(polygonLength * y); // / length);  //z
            //normals
        //    meshData.vboData.emplace_back(0.f);
        //    meshData.vboData.emplace_back(0.f);
        //    meshData.vboData.emplace_back(1.f);
        //    //textures
        //    meshData.vboData.emplace_back(0.f);
        //    meshData.vboData.emplace_back(0.f);
        }
    }

    std::cout << "vertices\n";
    for (int i = 0; i < meshData.vboData.size(); i++)
    {
        std::cout << meshData.vboData[i] << ",";
    }
        std::cout << std::endl;

    //indices
    for(int y = 0; y < divPerSide; y++)
    {
        for(int x = 0; x < divPerSide; x++)
        {
            //verts
            meshData.indices.emplace_back((divPerSide + 1) * y + x);
            meshData.indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 1);
            meshData.indices.emplace_back((divPerSide + 1) * y + x + 1);

            meshData.indices.emplace_back((divPerSide + 1) * y + x + 1);
            meshData.indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 1);
            meshData.indices.emplace_back((divPerSide + 1) * y + divPerSide + x + 2);
        }
   }
    std::cout << "indices\n";
    for (int i = 0; i < meshData.indices.size(); i++)
    {
        std::cout << meshData.indices[i] << ",";
    }
        std::cout << std::endl;

   // generatedMesh.emplace_back(0.f);  //x
   // generatedMesh.emplace_back(0.f);  //y
   // generatedMesh.emplace_back(0.f);  //z
   // for(int y = 0; y < divPerSide; y++) //y
   // {
   //     // Adds diagonal points
   //     for(int x = 0; x < divPerSide; x++) //x
   //     {
   //         generatedMesh.emplace_back(polygonLength * x + polygonLength);
   //         generatedMesh.emplace_back(polygonLength * y);  //y
   //         generatedMesh.emplace_back(0.f);  //z

   //         generatedMesh.emplace_back(polygonLength * x);
   //         generatedMesh.emplace_back(polygonLength * y + polygonLength);
   //         generatedMesh.emplace_back(0.f);  //z
   //     }
   //     //bottom right vertex in the row
   //     generatedMesh.emplace_back(length);  //x
   //     generatedMesh.emplace_back(polygonLength * y + polygonLength);  //y
   //     generatedMesh.emplace_back(0.f);  //z
   // }
}
};
