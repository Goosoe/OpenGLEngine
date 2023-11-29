#pragma once

// System headers
#include <glad/glad.h>

// Standard headers
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

//
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

//TODO: this can be mostly "staticified", we only care about the id of the shader mostly
//Make a struct without any functions and add a namespace then?
struct ShaderData
{
        GLuint program;
        GLint  status = 0;
        GLint  length = 0;
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
        //Shader()            { program = glCreateProgram(); }
        //Shader(std::string const &vertexFilename, std::string const &fragmentFilename){ 
        //    program = glCreateProgram();
        //    makeBasicShader(vertexFilename, fragmentFilename);
        //}

        GLuint createProgram();
        // Public member functions

        void activate(GLuint shaderProgram);

        void deactivate(); 
        
        //void deactivate() { glUseProgram(0); }

        //const GLuint get()        { return program; }

        void destroy(GLuint shaderProgram);

        //void destroy(GLuint shaderProgram)    { glDeleteProgram(shaderProgram); }
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

        /* Helper function for creating shaders */

       // void setCameraUniforms(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
       // {
       //     glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
       //     glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
       //     glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
       // }

       // // ------------------------------------------------------------------------
       // void setBool(const std::string &name, bool value) const
       // {         
       //     glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value); 
       // }
       // // ------------------------------------------------------------------------
       // void setInt(const std::string &name, int value) const
       // { 
       //     glUniform1i(glGetUniformLocation(program, name.c_str()), value); 
       // }
       // // ------------------------------------------------------------------------
       // void setFloat(const std::string &name, float value) const
       // { 
       //     glUniform1f(glGetUniformLocation(program, name.c_str()), value); 
       // }
       // // ------------------------------------------------------------------------
       // void setVec2(const std::string &name, const glm::vec2 &value) const
       // { 
       //     glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]); 
       // }
       // void setVec2(const std::string &name, float x, float y) const
       // { 
       //     glUniform2f(glGetUniformLocation(program, name.c_str()), x, y); 
       // }
       // // ------------------------------------------------------------------------
       // void setVec3(const std::string &name, const glm::vec3 &value) const
       // { 
       //     glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]); 
       // }
       // void setVec3(const std::string &name, float x, float y, float z) const
       // { 
       //     glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z); 
       // }
       // // ------------------------------------------------------------------------
       // void setVec4(const std::string &name, const glm::vec4 &value) const
       // { 
       //     glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]); 
       // }
       // void setVec4(const std::string &name, float x, float y, float z, float w) 
       // { 
       //     glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w); 
       // }
       // // ------------------------------------------------------------------------
       // void setMat2(const std::string &name, const glm::mat2 &mat) const
       // {
       //     glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
       // }
       // // ------------------------------------------------------------------------
       // void setMat3(const std::string &name, const glm::mat3 &mat) const
       // {
       //     glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
       // }
       // // ------------------------------------------------------------------------
       // void setMat4(const std::string &name, const glm::mat4 &mat) const
       // {
       //     glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
       // }

        void setCameraUniforms(GLuint shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection);

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
