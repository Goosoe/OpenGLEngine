#pragma once

#include "Model.h"

namespace ModelData
{
const std::vector<Vertex> cube
    {
        {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f)},
        {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
        {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },

    //     {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //
    //     {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //
    //     {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //
    //     {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(), glm::vec2(1.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(), glm::vec2(0.f), glm::vec3(0.f), glm::vec3(0.f) },
    //     {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(), glm::vec2(0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f) },
    };
}; //ModelData
