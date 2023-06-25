#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include "vertexArray.h"
#include "indexBuffer.h"
#include "shader.h"

#if defined(__APPLE__)
#define ASSERT(x) if(!(x)) __builtin_debugtrap();
#elif defined(_MSC_VER) 
#define ASSERT(x) if(!(x)) __debugbreak();
#endif

#if defined(NDEBUG)
#define GLCall(x) x;
#else
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};

#endif // !RENDERER_H
