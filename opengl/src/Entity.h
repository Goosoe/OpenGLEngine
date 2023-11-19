#pragma once
#include "glm/ext/vector_float3.hpp"
#include "utils/Shader.h"

/**
 * Responsible to hold data for basic entities. Has functions for the shader too
 */
class Entity
{
    private:
        // projection matrix
        glm::mat4 projection;
        // model matrix
        glm::mat4 model;
        //location of the entity on the scene
        glm::vec3 location;
        // scale of the object on the scene
        glm::vec3 scale;
        //TODO: add draw function for the shader and add a variable to hold the number of triangles

    public:
        Utils::Shader shader;

        Entity(glm::mat4 projection, glm::vec3 location = glm::vec3(0.f,0.f,0.f), glm::vec3 scale = glm::vec3(1.f,1.f,1.f));

        const glm::mat4& getEntityMat() const { return model; }

        void setLocation(glm::vec3& location);

        void setScale(glm::vec3& scale);

        void setProjection(glm::mat4& projection);
        
        /**
         *  Updates the camera uniforms in the attached shader. Uses its own model matrix and 
         *  set projection matrix
         */
        void updateCameraUniforms(glm::mat4& view);

        /**
         *  Updates the camera uniforms in the attached shader. Uses its own model matrix
         */
        void updateCameraUniforms(glm::mat4& view, glm::mat4& projection);

        //todo: make these values to be stored in entity
        void updateLightingUniforms(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular);

    private:
        inline void recalculateModelMatrix();

};
