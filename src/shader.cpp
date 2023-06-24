#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "renderer.h"




Shader::Shader(const std::string& filepath)
    : m_filePath(filepath), m_RendererID(0), m_uniformLocationCache()
{
    ShaderProgramSource shaderSource = parseShader(filepath);
    m_RendererID = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = getUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v3));
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
    {
        return m_uniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist. " << std::endl;
    }
    else
    {
        m_uniformLocationCache[name] = location;
    }


    return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream iftrm(filepath);
    std::string line;
    ShaderType type = ShaderType::NONE;
    std::ostringstream ss[2];
    
    while (std::getline(iftrm, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else if (type != ShaderType::NONE)
        {
            ss[type] << line << '\n';
        }
    }

    ShaderProgramSource src;
    src.VertexSource = ss[(int)ShaderType::VERTEX].str();
    src.FragmentSource = ss[(int)ShaderType::FRAGMENT].str();

    return src;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}