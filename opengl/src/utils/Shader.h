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
class Shader
{
    private:

        // Disable copying and assignment
       // Shader(Shader const &) = delete;
       // Shader& operator =(Shader const &) = delete;

    public:
        GLuint program;
        GLint  status;
        GLint  length;

        enum ShaderType{
            Vertex = 0,
            Fragment,
            Geometry,
            TessControl,
            TessEval,
            Compute
        };

        //Shader()            { program = glCreateProgram(); }
        Shader(std::string const &vertexFilename, std::string const &fragmentFilename){ 
            program = glCreateProgram();
            makeBasicShader(vertexFilename, fragmentFilename);
        }

        // Public member functions
        void activate()   { glUseProgram(program); }

        static void activate(GLuint shaderProgram)   { glUseProgram(shaderProgram); }

        static void deactivate() { glUseProgram(0); }
        
        //static void deactivate() { glUseProgram(0); }

        const GLuint get()        { return program; }

        void destroy()    { glDeleteProgram(program); }

        static void destroy(GLuint shaderProgram)    { glDeleteProgram(shaderProgram); }

        /* Attach a shader to the current shader program */
        void attach(std::string const &filename, ShaderType type)
        {
            // Load GLSL Shader from source
            std::ifstream fd(filename.c_str());
            if (fd.fail())
            {
                fprintf(stderr,
                        "Something went wrong when attaching the Shader file at \"%s\".\n"
                        "The file may not exist or is currently inaccessible.\n",
                        filename.c_str());
                return;
            }
            auto src = std::string(std::istreambuf_iterator<char>(fd),
                    (std::istreambuf_iterator<char>()));

            // Create shader object
            const char * source = src.c_str();
            auto shader = create(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            // Display errors
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (!status)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<char[]> buffer(new char[length]);
                glGetShaderInfoLog(shader, length, nullptr, buffer.get());
                fprintf(stderr, "%s\n%s", filename.c_str(), buffer.get());
            }

            assert(status);

            // Attach shader and free allocated memory
            glAttachShader(program, shader);
            glDeleteShader(shader);
        }

        /* Links all attached shaders together into a shader program */
        void link()
        {
            // Link all attached shaders
            glLinkProgram(program);

            // Display errors
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if (!status)
            {
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<char[]> buffer(new char[length]);
                glGetProgramInfoLog(program, length, nullptr, buffer.get());
                fprintf(stderr, "%s\n", buffer.get());
            }

            assert(status);
        }

        /* Convenience function that attaches and links a vertex and a
           fragment shader in a shader program */
        void makeBasicShader(std::string const &vertexFilename,
                std::string const &fragmentFilename)
        {
            attach(vertexFilename, Vertex);
            attach(fragmentFilename, Fragment);
            link();
        }

        /* Used for debugging shader programs (expensive to run) */
        bool isValid()
        {
            // Validate linked shader program
            glValidateProgram(program);

            // Display errors
            glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
            if (!status)
            {
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<char[]> buffer(new char[length]);
                glGetProgramInfoLog(program, length, nullptr, buffer.get());
                fprintf(stderr, "%s\n", buffer.get());
                return false;
            }
            return true;
        }

        /* Helper function for creating shaders */
        GLuint create(ShaderType type)
        {
            switch(type)
            {
                case Fragment:
                    return glCreateShader(GL_FRAGMENT_SHADER);
                case Vertex:
                    return glCreateShader(GL_VERTEX_SHADER);
                case Geometry:
                    return glCreateShader(GL_GEOMETRY_SHADER);
                case TessControl:
                    return glCreateShader(GL_TESS_CONTROL_SHADER);
                case TessEval:   
                    return glCreateShader(GL_TESS_EVALUATION_SHADER);
                case Compute:    
                    return glCreateShader(GL_VERTEX_SHADER);
                default:
                    return false;
            }
        }

        void setCameraUniforms(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
        {
            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        }

        // ------------------------------------------------------------------------
        void setBool(const std::string &name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value); 
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const
        { 
            glUniform1i(glGetUniformLocation(program, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const
        { 
            glUniform1f(glGetUniformLocation(program, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const
        { 
            glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]); 
        }
        void setVec2(const std::string &name, float x, float y) const
        { 
            glUniform2f(glGetUniformLocation(program, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        { 
            glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]); 
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        { 
            glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        { 
            glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]); 
        }
        void setVec4(const std::string &name, float x, float y, float z, float w) 
        { 
            glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

        static void setCameraUniforms(GLuint shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        }

        // ------------------------------------------------------------------------
        static void setBool(GLuint shaderProgram, const std::string &name, bool value)
        {         
            glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
        }
        // ------------------------------------------------------------------------
        static void setInt(GLuint shaderProgram, const std::string &name, int value)
        { 
            glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        static void setFloat(GLuint shaderProgram, const std::string &name, float value) 
        { 
            glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        static void setVec2(GLuint shaderProgram, const std::string &name, const glm::vec2 &value) 
        { 
            glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec2(GLuint shaderProgram, const std::string &name, float x, float y)
        { 
            glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        static void setVec3(GLuint shaderProgram, const std::string &name, const glm::vec3 &value)
        { 
            glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
        }
        static void setVec3(GLuint shaderProgram, const std::string &name, float x, float y, float z)
        { 
            glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        static void setVec4(GLuint shaderProgram, const std::string &name, const glm::vec4 &value) 
        { 
            glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
        }
        static void setVec4(GLuint shaderProgram, const std::string &name, float x, float y, float z, float w) 
        { 
            glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        static void setMat2(GLuint shaderProgram, const std::string &name, const glm::mat2 &mat)
        {
            glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        static void setMat3(GLuint shaderProgram, const std::string &name, const glm::mat3 &mat)
        {
            glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        static void setMat4(GLuint shaderProgram, const std::string &name, const glm::mat4 &mat)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
};
