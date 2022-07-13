#include "FrameBuffer.hpp"
#include <glad/glad.h>

#include <cmath>

#include <assert.h>

FearEngine::Render::FrameBuffer::FrameBuffer()
 : frameBufferId(-1)
 , depthId(-1)
 , stencilId(-1)
 , initialized(false)
 , enabled(0)
{
	for (int i = 0; i < colors.size(); ++i)
	{
		colors[i] = -1;
		data.clearValues[i] = defaultClear;
	}
}

FearEngine::Render::FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept { *this = std::move(other); }

FearEngine::Render::FrameBuffer& FearEngine::Render::FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
	data = other.data;
	depthId = other.depthId;
	stencilId = other.stencilId;
	frameBufferId = other.frameBufferId;
	initialized = other.initialized;
	enabled = other.enabled;

	other.depthId = -1;
	other.stencilId = -1;
	other.frameBufferId = -1;

	colors = other.colors;

	for (int i = 0; i < colors.size(); ++i)
	{
		other.colors[i] = -1;
	}

	other.initialized = false;

	return *this;
}

void FearEngine::Render::FrameBuffer::init(const FrameBufferParams& params) { setParams(params); }

uint32_t FearEngine::Render::FrameBuffer::getDepthAttachment() const
{
	assert(depthId != -1);
	return depthId;
}

uint32_t FearEngine::Render::FrameBuffer::getStencilAttachment() const
{
	if (data.bufferTypes & FrameBufferType::Stencil && data.bufferTypes & FrameBufferType::Depth)
	{
		assert(depthId != -1);
		return depthId;
	}

	assert(stencilId != -1);
	return stencilId;
}

uint32_t FearEngine::Render::FrameBuffer::getColorAttachment(uint8_t attachNum) const
{
	assert(attachNum < FrameBufferParams::maxColorAttachments);
	assert(colors[attachNum] != -1);

	return colors[attachNum];
}

void FearEngine::Render::FrameBuffer::bindColorAttachment(uint8_t attachNum, uint8_t textureUnit)
{
	glBindTextureUnit(textureUnit, getColorAttachment(attachNum));
}

const FearEngine::Render::FrameBufferParams& FearEngine::Render::FrameBuffer::getParams() const { return data; }

void FearEngine::Render::FrameBuffer::setParams(const FrameBufferParams& params)
{
	initialized = true;
	data = params;
	onResize();
}

uint32_t getInternalFormat(FearEngine::Render::ColorFormats type)
{
	switch (type)
	{
	case FearEngine::Render::ColorFormat::R8:
	case FearEngine::Render::ColorFormat::R16:
	case FearEngine::Render::ColorFormat::R32F:
		return GL_R;
	case FearEngine::Render::ColorFormat::RG8:
	case FearEngine::Render::ColorFormat::RG16:
	case FearEngine::Render::ColorFormat::RG32F:
		return GL_RG;
	case FearEngine::Render::ColorFormat::RGB8:
	case FearEngine::Render::ColorFormat::RGB16:
	case FearEngine::Render::ColorFormat::RGB32F:
		return GL_RGB;
	case FearEngine::Render::ColorFormat::RGBA8:
	case FearEngine::Render::ColorFormat::RGBA16:
	case FearEngine::Render::ColorFormat::RGBA32F:
		return GL_RGBA;
	case FearEngine::Render::ColorFormat::R32:
		return GL_RED_INTEGER;
	default:
		return 0;
	}
}

glm::vec4 FearEngine::Render::FrameBuffer::getPixel(FrameBufferTypes bufferType, glm::vec2& pos)
{
	uint32_t type = -1;
	uint32_t format;

	if (bufferType & (FrameBufferType::Depth | FrameBufferType::Stencil))
	{
		type = GL_DEPTH_STENCIL_ATTACHMENT;
		format = GL_DEPTH24_STENCIL8;
	}
	else if (bufferType & FrameBufferType::Depth)
	{
		type = GL_DEPTH_ATTACHMENT;
		format = data.depthFormat;
	}
	else if (bufferType & FrameBufferType::Stencil)
	{
		type = GL_STENCIL_ATTACHMENT;
		format = data.stencilFormat;
	}
	else
	{
		int16_t pos = log2(bufferType) - log2(static_cast<uint64_t>(FrameBufferType::ColorAttachment0));
		assert(pos >= log2(static_cast<uint64_t>(FrameBufferType::ColorAttachment0))
			&& pos < FrameBufferParams::maxColorAttachments && "Undefined framebuffer type");

		type = GL_COLOR_ATTACHMENT0 + pos;
		format = data.colorFormat[pos];
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	glReadBuffer(type);
	uint32_t pixelData = 0;

	// Todo add ability to set read internal size
	glReadPixels(pos.x, pos.y, 1, 1, getInternalFormat(format), GL_INT, &pixelData);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	char* data = reinterpret_cast<char*>(&pixelData);
	return glm::vec4(data[0], data[1], data[2], data[2]);
}

void FearEngine::Render::FrameBuffer::onResize()
{
	if (!initialized)
	{
		return;
	}

	if (frameBufferId == -1)
	{
		glGenFramebuffers(1, &frameBufferId);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	uint64_t colorAttachment = FrameBufferType::ColorAttachment0;
	for (uint8_t i = 0; i < colors.size(); ++i)
	{
		if (data.bufferTypes & colorAttachment)
		{
			if (colors[i] == -1)
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &colors[i]);
			}

			glBindTexture(GL_TEXTURE_2D, colors[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, data.colorFormat[i], data.width, data.height, 0, getInternalFormat(data.colorFormat[i]),
				 GL_UNSIGNED_BYTE, nullptr);

			// Todo think about adding ability to filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colors[i], 0);
		}

		colorAttachment = colorAttachment << 1;
	}

	if (data.bufferTypes & FrameBufferType::Depth && !(data.bufferTypes & FrameBufferType::Stencil))
	{
		if (depthId == -1)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &depthId);
		}

		glBindTexture(GL_TEXTURE_2D, depthId);
		glTexImage2D(GL_TEXTURE_2D, 0, data.depthFormat, data.width, data.height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthId, 0);
	}

	if (data.bufferTypes & FrameBufferType::Stencil && !(data.bufferTypes & FrameBufferType::Depth))
	{
		if (stencilId == -1)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &stencilId);
		}

		glBindTexture(GL_TEXTURE_2D, stencilId);
		glTexImage2D(GL_TEXTURE_2D, 0, data.stencilFormat, data.width, data.height, 0, getInternalFormat(data.colorFormat[0]),
			 GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilId, 0);
	}

	if (data.bufferTypes & FrameBufferType::Stencil && data.bufferTypes & FrameBufferType::Depth)
	{
		uint32_t dataType = 0;
		uint32_t dataFormat = 0;

		if (data.depthFormat == DepthFormat::Depth24 && data.stencilFormat == StencilFormat::Stencil8)
		{
			dataType = GL_DEPTH24_STENCIL8;
			dataFormat = GL_UNSIGNED_INT_24_8;
		}
		else if (data.depthFormat == DepthFormat::Depth32F && data.stencilFormat == StencilFormat::Stencil8)
		{
			dataType = GL_DEPTH32F_STENCIL8;
			dataFormat = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		}
		else
		{
			assert(!"Depth format and stencil supports only 24_8 and 32F_8");
		}

		if (depthId == -1)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &depthId);
		}

		glBindTexture(GL_TEXTURE_2D, depthId);
		glTexImage2D(GL_TEXTURE_2D, 0, dataType, data.width, data.height, 0, GL_DEPTH_STENCIL, dataFormat, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthId, 0);
	}

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "FrameBuffer creation has failed!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FearEngine::Render::FrameBuffer::enable(uint32_t* skipedColorAttachmentNum, uint8_t count, bool disableDepth, bool cl)
{
	glViewport(0, 0, data.width, data.height);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	if (cl)
	{
		clear();
	}

	if (!disableDepth && depthId != -1)
	{
		if (data.bufferTypes & FrameBufferType::Stencil)
		{
			glEnable(GL_STENCIL_TEST);
		}

		glEnable(GL_DEPTH_TEST);
	}

	if (stencilId != -1)
	{
		glEnable(GL_STENCIL_TEST);
	}

	std::array<uint32_t, FrameBufferParams::maxColorAttachments> buffer;
	uint32_t enabledAttachments = 0;
	for (uint8_t i = 0; i < colors.size(); ++i)
	{
		if (colors[i] == -1)
		{
			continue;
		}

		bool skip = skipedColorAttachmentNum == nullptr;
		for (uint8_t j = 0; j < count && !skip; ++j)
		{
			skip = skipedColorAttachmentNum[j] == i;
		}

		if (skipedColorAttachmentNum == nullptr || !skip)
		{
			buffer[enabledAttachments] = GL_COLOR_ATTACHMENT0 + i;
			++enabledAttachments;
		}
	}

	glDrawBuffers(enabledAttachments, buffer.data());
}

void FearEngine::Render::FrameBuffer::disable()
{
	if (depthId != -1)
	{
		if (data.bufferTypes & FrameBufferType::Stencil)
		{
			glDisable(GL_STENCIL_TEST);
		}

		glDisable(GL_DEPTH_TEST);
	}

	if (stencilId != -1)
	{
		glDisable(GL_STENCIL_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FearEngine::Render::FrameBuffer::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	glClear(GL_COLOR_BUFFER_BIT | (GL_DEPTH_BUFFER_BIT * (depthId != -1)) | (GL_STENCIL_BUFFER_BIT * (stencilId != -1)));

	for (uint8_t i = 1; i < colors.size(); ++i)
	{
		if (colors[i] != -1)
		{
			if (data.colorFormat[i] == ColorFormat::R32)
			{
				int value = data.clearValues[i];
				glClearTexImage(colors[i], 0, getInternalFormat(data.colorFormat[i]), GL_INT, &value);
			}
			else
			{
				glClearTexImage(colors[i], 0, getInternalFormat(data.colorFormat[i]), GL_UNSIGNED_BYTE, &data.clearValues[i]);
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FearEngine::Render::FrameBuffer::isInitialized() const { return initialized; }

FearEngine::Render::FrameBuffer::~FrameBuffer()
{
	for (auto id : colors)
	{
		if (id != -1)
		{
			glDeleteTextures(1, &id);
		}
	}

	if (depthId == -1)
	{
		glDeleteTextures(1, &depthId);
	}

	if (stencilId == -1)
	{
		glDeleteTextures(1, &stencilId);
	}

	if (frameBufferId != -1)
	{
		glDeleteFramebuffers(1, &frameBufferId);
	}
}
