#include <iostream>
#include "renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    // OpenGL guarantee GL_NO_ERROR always equals to 0
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]("
            << error << "):"
            << function << " "
            << file << ":"
            << line
            << std::endl;
        return false;
    }

    return true;
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.bind();
    va.bind();
    ib.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}