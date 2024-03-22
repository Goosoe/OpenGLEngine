#pragma once

#include "Model.h"

namespace ModelData
{
const std::vector<Vertex> cube
    {
        //left
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-1.f, 0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.f, 0.f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f)},
        {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-1.f, 0.f, 0.f), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-1.f, 0.f, 0.f), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.f, 0.f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        //right
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
 
        //front
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },

        //back
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },

        //bottom
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },

        //top
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    };
}; //ModelData
