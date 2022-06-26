#include <glad/glad.h>
#include "Texture.hpp"

#include <utils/PointerCasts.hpp>

FearEngine::Render::Texture::Texture()
 : textureId(-1)
{}

FearEngine::Render::Texture::~Texture()
{
	if (textureId != -1)
	{
		glDeleteTextures(1, &textureId);
	}
}

FearEngine::Render::Texture::Texture(Texture&& tex) noexcept
{
	texRes = tex.texRes;
	textureId = tex.textureId;
	tex.textureId = -1;
}

FearEngine::Render::Texture& FearEngine::Render::Texture::operator=(Texture&& tex) noexcept
{
	texRes = tex.texRes;
	textureId = tex.textureId;
	tex.textureId = -1;

	return *this;
}

// Todo think about caching mipmap to reduce texture loading time
void FearEngine::Render::Texture::init(std::shared_ptr<Cache::Resource>& resource)
{
	texRes = resource;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto extra = utils::reinterpret_pointer_cast<Cache::ImageData>(resource->extra);

	assert(resource->size != 0);

	GLint format;
	switch (extra->channels)
	{
	case 1:
		format = GL_R;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		format = 0;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, extra->width, extra->height, 0, format, GL_UNSIGNED_BYTE, resource->data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

uint32_t FearEngine::Render::Texture::getTexHandle() const { return textureId; }
const std::shared_ptr<FearEngine::Cache::Resource>& FearEngine::Render::Texture::getResource() const { return texRes; };

#include <core/Engine.hpp>
void FearEngine::Render::Texture::enable(uint8_t slot)
{
	glBindTextureUnit(slot, textureId);
}

void FearEngine::Render::Texture::disable()
{}

