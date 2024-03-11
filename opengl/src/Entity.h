#pragma once
#include "Shader.h"

struct RotationData
{
    float angle;
    glm::vec3 rotationAxis;
};

/**
 * Entity present in a scene. Holds data of a single entity
 */
class Entity
{
    private:
        // projection matrix
        glm::mat4 projectionMatrix;
        // model matrix
        glm::mat4 modelMatrix;
        // normal matrix - in case we need it, it is here
        //glm::mat3 normalMatrix;
        // scale of the object on the scene
        glm::vec3 scale;
        // rotation of the object on the scene
        RotationData rotationData;
        //location of the entity on the scene
        glm::vec3 location;

    public:
        
        //stores the shader program Id it will use
        GLuint shaderProgram;

        // TODO: do we want to have the auto shader activation here? The rest of the functions don't do that
        // Activates shader program, calculates necessary data and sets the values to the shaders
        // The rest of the setters are unprotected and requires the user to activate the shader for himself
        Entity(GLuint shaderProgram, glm::mat4 projection, glm::vec3 scale = glm::vec3(1.f,1.f,1.f), 
                glm::vec3 location = glm::vec3(0.f,0.f,0.f), RotationData = {0.f, glm::vec3(1.f)});

        const glm::mat4& getModelMat() const { return modelMatrix; }

        void setLocation(glm::vec3& location);

        void setScale(glm::vec3& scale);

        void setRotation(RotationData& rotation);

        void setProjection(glm::mat4& projection);
        
        /**
         *  Updates the view uniform in the attached shader. 
         */
        void setViewUniform(glm::mat4& view);

        /**
         *  Updates the projection uniform in the attached shader. 
         */
        void setProjectionUniform(glm::mat4& projection);

        //TODO: make these values to be stored in entity - IMPLEMENT THIS AGAIN?
        // void updateLightingUniforms(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular);
        

    private:
        void recalculateModelMatrix();
};
