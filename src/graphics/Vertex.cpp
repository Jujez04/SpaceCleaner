#include "graphics/Vertex.h"

vrtx::VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &rendererId);
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}