#include "FrameBuffer.hpp"
#include <glad/glad.h>

#include <assert.h>

FearEngine::Render::FrameBuffer::FrameBuffer()
 : frameBufferId(-1)
 , colorId(-1)
 , depthId(-1)
 , stencilId(-1)
 , initialized(false)
{}

FearEngine::Render::FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
{
	*this = std::move(other);
}

FearEngine::Render::FrameBuffer& FearEngine::Render::FrameBuffer::operator=(FrameBuffer&& other) noexcept
{ 
	data = other.data;
	colorId = other.colorId;
	depthId = other.depthId;
	stencilId = other.stencilId;
	frameBufferId = other.frameBufferId;
	initialized = other.initialized;
	enabled = other.enabled;

	other.colorId = -1;
	other.depthId = -1;
	other.stencilId = -1;
	other.frameBufferId = -1;

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

uint32_t FearEngine::Render::FrameBuffer::getColorAttachment() const
{
	assert(colorId != -1);
	return colorId;
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
	default:
		return 0;
	}
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

	if (data.bufferTypes & FrameBufferType::Color)
	{
		if (colorId == -1)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &colorId);
		}

		glBindTexture(GL_TEXTURE_2D, colorId);
		glTexImage2D(GL_TEXTURE_2D, 0, data.colorFormat, data.width, data.height, 0, getInternalFormat(data.colorFormat), GL_UNSIGNED_BYTE, nullptr);

		// Todo think about adding ability to filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Todo add ability to change Color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorId, 0);
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
		glTexImage2D(GL_TEXTURE_2D, 0, data.stencilFormat, data.width, data.height, 0, getInternalFormat(data.colorFormat), GL_UNSIGNED_BYTE, nullptr);
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

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "FrameBuffer cration failed!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FearEngine::Render::FrameBuffer::enable()
{
	enabled = FrameBufferType::None;
	if (glIsEnabled(GL_DEPTH_TEST))
	{
		enabled |= FrameBufferType::Depth;
	}

	if (glIsEnabled(GL_STENCIL_TEST))
	{
		enabled |= FrameBufferType::Stencil;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	if (depthId != -1)
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

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FearEngine::Render::FrameBuffer::disable() {

	if (depthId != -1)
	{
		if (data.bufferTypes & FrameBufferType::Stencil && !(enabled & FrameBufferType::Stencil))
		{
			glDisable(GL_STENCIL_TEST);
		}

		if (!(enabled & FrameBufferType::Depth))
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (stencilId != -1 && !(enabled & FrameBufferType::Stencil))
	{
		glDisable(GL_STENCIL_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

bool FearEngine::Render::FrameBuffer::isInitialized() const { return initialized; }

FearEngine::Render::FrameBuffer::~FrameBuffer()
{
	if (colorId == -1)
	{
		glDeleteTextures(1, &colorId);
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
