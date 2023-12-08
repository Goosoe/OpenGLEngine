#include "Shader.h"
#include <memory>
#include <cassert>
#include <fstream>

namespace Shader
{
        GLuint createProgram()   { return glCreateProgram(); }
        // Public member functions

        void activate(GLuint shaderProgram)   { glUseProgram(shaderProgram); }

        void deactivate() { glUseProgram(0); }

        void destroy(GLuint shaderProgram)    { glDeleteProgram(shaderProgram); }

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

        /* Attach a shader to the current shader program */
        void attach(ShaderData& shaderData, std::string const &filename, ShaderType type)
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
            glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderData.status);
            if (!shaderData.status)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderData.length);
                std::unique_ptr<char[]> buffer(new char[shaderData.length]);
                glGetShaderInfoLog(shader, shaderData.length, nullptr, buffer.get());
                fprintf(stderr, "%s\n%s", filename.c_str(), buffer.get());
            }

            assert(shaderData.status);

            // Attach shader and free allocated memory
            glAttachShader(shaderData.program, shader);
            glDeleteShader(shader);
        }

        /* Links all attached shaders together into a shader program */
        void link(ShaderData& shaderData)
        {
            // Link all attached shaders
            glLinkProgram(shaderData.program);

            // Display errors
            glGetProgramiv(shaderData.program, GL_LINK_STATUS, &shaderData.status);
            if (!shaderData.status)
            {
                glGetProgramiv(shaderData.program, GL_INFO_LOG_LENGTH, &shaderData.length);
                std::unique_ptr<char[]> buffer(new char[shaderData.length]);
                glGetProgramInfoLog(shaderData.program, shaderData.length, nullptr, buffer.get());
                fprintf(stderr, "%s\n", buffer.get());
            }

            assert(shaderData.status);
        }

        /* Convenience function that attaches and links a vertex and a
           fragment shader in a shader program */
        void makeBasicShader(ShaderData& shaderData, std::string const &vertexFilename,
                std::string const &fragmentFilename)
        {
            attach(shaderData, vertexFilename, Vertex);
            attach(shaderData, fragmentFilename, Fragment);
            link(shaderData);
        }

        /* Used for debugging shader programs (expensive to run) */
        bool isValid(ShaderData& shaderData)
        {
            // Validate linked shader program
            glValidateProgram(shaderData.program);

            // Display errors
            glGetProgramiv(shaderData.program, GL_VALIDATE_STATUS, &shaderData.status);
            if (!shaderData.status)
            {
                glGetProgramiv(shaderData.program, GL_INFO_LOG_LENGTH, &shaderData.length);
                std::unique_ptr<char[]> buffer(new char[shaderData.length]);
                glGetProgramInfoLog(shaderData.program, shaderData.length, nullptr, buffer.get());
                fprintf(stderr, "%s\n", buffer.get());
                return false;
            }
            return true;
        }

        void setModelUniform(GLuint shaderProgram, glm::mat4& model)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glm::mat3 normalMat = transpose(inverse(glm::mat3(model)));
            glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));
        }

        void setViewUniform(GLuint shaderProgram, glm::mat4& view)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        }

        void setProjectionUniform(GLuint shaderProgram, glm::mat4& projection)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        }

        void setMVPUniforms(GLuint shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
        {
            setModelUniform(shaderProgram, model);
            setViewUniform(shaderProgram, view);
            setProjectionUniform(shaderProgram, projection);
        }

        // ------------------------------------------------------------------------
        void setBool(GLuint shaderProgram, const std::string &name, bool value)
        {         
            glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
        }
        // ------------------------------------------------------------------------
        void setInt(GLuint shaderProgram, const std::string &name, int value)
        { 
            glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setFloat(GLuint shaderProgram, const std::string &name, float value) 
        { 
            glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
        }
        // ------------------------------------------------------------------------
        void setVec2(GLuint shaderProgram, const std::string &name, const glm::vec2 &value) 
        { 
            glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec2(GLuint shaderProgram, const std::string &name, float x, float y)
        { 
            glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y); 
        }
        // ------------------------------------------------------------------------
        void setVec3(GLuint shaderProgram, const std::string &name, const glm::vec3 &value)
        { 
            glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec3(GLuint shaderProgram, const std::string &name, float x, float y, float z)
        { 
            glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); 
        }
        // ------------------------------------------------------------------------
        void setVec4(GLuint shaderProgram, const std::string &name, const glm::vec4 &value) 
        { 
            glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
        }
        void setVec4(GLuint shaderProgram, const std::string &name, float x, float y, float z, float w) 
        { 
            glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w); 
        }
        // ------------------------------------------------------------------------
        void setMat2(GLuint shaderProgram, const std::string &name, const glm::mat2 &mat)
        {
            glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(GLuint shaderProgram, const std::string &name, const glm::mat3 &mat)
        {
            glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(GLuint shaderProgram, const std::string &name, const glm::mat4 &mat)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
};
