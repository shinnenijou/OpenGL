#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vertexBuffer.h>

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_rendererID;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void bind() const;
    void unbind()const;
};

#endif
