#pragma once

// System headers
#include <glad/glad.h>

// Standard headers
#include <string>
#include <unordered_map>

#include "glm/ext/matrix_float4x4.hpp"

/**
 * Has functions and data structures relevant to interacting with shaders.
 * The responsibility to set the active shader is up to the user.
 */

struct ShaderData
{
    GLuint program;
    GLint status = 0;
    GLint length = 0;
};

namespace Shader
{

    enum ShaderType{
        Vertex = 0,
        Fragment,
        Geometry,
        TessControl,
        TessEval,
        Compute
    };

        GLuint createProgram();

        void activate(GLuint shaderProgram);

        void deactivate(); 
        
        void destroy(GLuint shaderProgram);

        GLuint create(ShaderType type);

        /* Attach a shader to the current shader program */
        void attach(ShaderData& shaderData, std::string const &filename, ShaderType type);

        /* Links all attached shaders together into a shader program */
        void link(ShaderData& shaderData);

        /* Convenience function that attaches and links a vertex and a
           fragment shader in a shader program */
        void makeBasicShader(ShaderData& shaderData, std::string const &vertexFilename,
                std::string const &fragmentFilename);

        /* Used for debugging shader programs (expensive to run) */
        bool isValid(ShaderData& shaderData);

        //Sets the model uniform and updates and sets the normal matrix uniform
        void setModelUniform(GLuint shaderProgram, glm::mat4& model);

        void setViewUniform(GLuint shaderProgram, glm::mat4& view);

        void setProjectionUniform(GLuint shaderProgram, glm::mat4& projection);

        // Sets Model, View and Projection Uniform
        void setMVPUniforms(GLuint shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection);

        /* Helper function for setting shader uniforms */
        // ------------------------------------------------------------------------
        void setBool(GLuint shaderProgram, const std::string &name, bool value);

        // ------------------------------------------------------------------------
        void setInt(GLuint shaderProgram, const std::string &name, int value);

        // ------------------------------------------------------------------------
        void setFloat(GLuint shaderProgram, const std::string &name, float value);

        // ------------------------------------------------------------------------
        void setVec2(GLuint shaderProgram, const std::string &name, const glm::vec2 &value);
        void setVec2(GLuint shaderProgram, const std::string &name, float x, float y);

        // ------------------------------------------------------------------------
        void setVec3(GLuint shaderProgram, const std::string &name, const glm::vec3 &value);
        void setVec3(GLuint shaderProgram, const std::string &name, float x, float y, float z);

        // ------------------------------------------------------------------------
        void setVec4(GLuint shaderProgram, const std::string &name, const glm::vec4 &value) ;
        void setVec4(GLuint shaderProgram, const std::string &name, float x, float y, float z, float w) ;
        // ------------------------------------------------------------------------
        void setMat2(GLuint shaderProgram, const std::string &name, const glm::mat2 &mat);

        // ------------------------------------------------------------------------
        void setMat3(GLuint shaderProgram, const std::string &name, const glm::mat3 &mat);

        // ------------------------------------------------------------------------
        void setMat4(GLuint shaderProgram, const std::string &name, const glm::mat4 &mat);
};
