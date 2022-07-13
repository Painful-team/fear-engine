#include <glad/glad.h>
#include "TextureBuffer.hpp"
#include <cassert>

namespace FearEngine::Render
{
TextureBuffer::TextureBuffer()
 : texture(-1)
 , buffer(-1)
{}

void TextureBuffer::genBuffer()
{
	if (buffer != -1)
	{
		return;
	}

	glGenBuffers(1, &buffer);
}

void TextureBuffer::bind(uint16_t unit) const
{
	glBindTexture(GL_TEXTURE_BUFFER, texture);
}
void TextureBuffer::reserveData(const uint32_t size)
{
	assert(buffer != -1 && "Buffer not initialized");

	glBindBuffer(GL_TEXTURE_BUFFER, buffer);
	glBufferData(GL_TEXTURE_BUFFER, size, nullptr, GL_STATIC_DRAW);

	if (texture == -1)
	{
		glGenTextures(1, &texture);
	}

	glBindTexture(GL_TEXTURE_BUFFER, texture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, buffer);

	unbind();
}
void TextureBuffer::setData(void* data, const uint32_t size)
{
	glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_BUFFER, texture);
	glBindBuffer(GL_TEXTURE_BUFFER, buffer);
	glBufferSubData(GL_TEXTURE_BUFFER, 0, size, data);
}

void TextureBuffer::unbind() const { glBindBuffer(GL_TEXTURE_BUFFER, 0); }
TextureBuffer::~TextureBuffer()
{
	if (texture != -1)
	{
		glDeleteTextures(1, &texture);
	}

	if (buffer != -1)
	{
		glDeleteBuffers(1, &buffer);
	}
}
}  // namespace FearEngine::Render
