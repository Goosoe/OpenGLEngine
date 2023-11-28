#include "Entity.h"

Entity::Entity(std::string path, glm::mat4 projection, glm::vec3 scale, glm::vec3 location) : 
    projectionMatrix(projection),
    modelMatrix(glm::mat4(1.f)),
    scale(scale),
    location(location),
    model(path)
{ 
    recalculateModelMatrix();
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

void Entity::setProjection(glm::mat4& projection)
{
    this->projectionMatrix = projection;   
}

void Entity::updateCameraUniforms(glm::mat4& view)
{
    shader.setCameraUniforms(modelMatrix, view, projectionMatrix);
}

void Entity::updateCameraUniforms(glm::mat4& view, glm::mat4& projection)
{
    shader.setCameraUniforms(modelMatrix, view, projection);
}

void Entity::draw()
{
    model.draw(shader);
}

void Entity::recalculateModelMatrix()
{
    modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, location);
    modelMatrix = glm::scale(modelMatrix, scale);
}
