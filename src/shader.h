#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    std::string m_filePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_uniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    // Set Uniform
    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    unsigned int getUniformLocation(const std::string& name);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderProgramSource parseShader(const std::string& filepath);

};

#endif // !SHADER_H
