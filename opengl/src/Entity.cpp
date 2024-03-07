#include "Entity.h"
#include "glm/ext/matrix_transform.hpp"

Entity::Entity(GLuint shaderProgram, glm::mat4 projection, glm::vec3 scale, glm::vec3 location, RotationData rotation) : 
    projectionMatrix(projection),
    modelMatrix(glm::mat4(1.f)),
    scale(scale),
    rotationData(rotation),
    location(location),
    shaderProgram(shaderProgram)
{ 
    glUseProgram(shaderProgram);
    Shader::setProjectionUniform(shaderProgram, projectionMatrix);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationData.angle), rotationData.rotationAxis);
    modelMatrix = glm::translate(modelMatrix, location);
    modelMatrix = glm::scale(modelMatrix, scale);
    Shader::setModelUniform(shaderProgram, modelMatrix);
    glUseProgram(0);
}

void Entity::setLocation(glm::vec3& location)
{
    this->location = location;
    recalculateModelMatrix();
}

void Entity::setScale(glm::vec3& scale)
{
    this->scale = scale;
    recalculateModelMatrix();
}

void Entity::setRotation(RotationData& rotation)
{
    this->rotationData = rotation;
    recalculateModelMatrix();
}

void Entity::setViewUniform(glm::mat4& view)
{
    Shader::setViewUniform(shaderProgram, view);
}

void Entity::setProjectionUniform(glm::mat4& projection)
{
    Shader::setProjectionUniform(shaderProgram, projection);
}

void Entity::recalculateModelMatrix()
{
    modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationData.angle), rotationData.rotationAxis);
    modelMatrix = glm::translate(modelMatrix, location);
    modelMatrix = glm::scale(modelMatrix, scale);
    //normalMatrix = transpose(inverse(glm::mat3(modelMatrix)));
    Shader::setModelUniform(shaderProgram, modelMatrix);
    //Shader::setNormalMatrix(shaderProgram, normalMatrix);
}
