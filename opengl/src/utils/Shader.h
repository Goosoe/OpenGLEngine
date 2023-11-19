#pragma once

// System headers
#include <glad/glad.h>

// Standard headers
#include <cassert>
#include <fstream>
#include <memory>
#include <string>

//
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Utils
{
    class Shader
    {
    private:
        GLuint program;
        GLint  status;
        GLint  length;

        // Disable copying and assignment
        Shader(Shader const &) = delete;
        Shader& operator =(Shader const &) = delete;

    public:

        enum ShaderType{
            Vertex = 0,
            Fragment,
            Geometry,
            TessControl,
            TessEval,
            Compute
        };

        Shader()            { program = glCreateProgram(); }

        // Public member functions
        void activate()   { glUseProgram(program); }

        void deactivate() { glUseProgram(0); }

        GLuint get()        { return program; }

        void destroy()    { glDeleteProgram(program); }

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
            // Extract file extension and create the correct shader type
            //auto idx = filename.rfind(".");
            //auto ext = filename.substr(idx + 1);
            //if (ext == "comp") return glCreateShader(GL_COMPUTE_SHADER);
            //else if (ext == "frag") return glCreateShader(GL_FRAGMENT_SHADER);
            //else if (ext == "geom") return glCreateShader(GL_GEOMETRY_SHADER);
            //else if (ext == "tcs")  return glCreateShader(GL_TESS_CONTROL_SHADER);
            //else if (ext == "tes")  return glCreateShader(GL_TESS_EVALUATION_SHADER);
            //else if (ext == "vert") return glCreateShader(GL_VERTEX_SHADER);
            //else                    return false;
        }

        void setCameraUniforms(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
        {
            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        }

        //TODO: create template for set uniform :)
    };
}
