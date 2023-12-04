#pragma once


// System headers
#include <vector>


// Level
void runTeapotLevel(struct GLFWwindow* window);

/**
 * iterates over shader vector and updates their common uniforms every frame
 */
void updateUniformsOfShaders(const std::vector<class ShaderData>& shaders, const class Camera& camera);
