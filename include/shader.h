#ifndef SHADER_H
#define SHADER_H

#include <string>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

const ShaderProgramSource parseShader(const std::string& source);
unsigned int compileShader(unsigned int type, const std::string& source);
int createShader(const std::string& vertexShader, const std::string& fragmentShader);

#endif // !SHADER_H
