#include "Entity.h"

Entity::Entity(glm::mat4 projection, glm::vec3 location, glm::vec3 scale) : 
    projection(projection),
    model(glm::mat4(1.f)),
    location(location),
    scale(scale)
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
    this->projection = projection;   
}

void Entity::updateCameraUniforms(glm::mat4& view)
{
    shader.setCameraUniforms(model, view, projection);
}

void Entity::updateCameraUniforms(glm::mat4& view, glm::mat4& projection)
{
    shader.setCameraUniforms(model, view, projection);
}

void Entity::recalculateModelMatrix()
{
    model = glm::mat4(1.f);
    model = glm::translate(model, location);
    model = glm::scale(model, scale);
}
