#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer
{
private:
	unsigned int m_rendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind();
	void unbind();
};

#endif // !VERTEXBUFFER_H
