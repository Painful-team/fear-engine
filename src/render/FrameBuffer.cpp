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

	if (colorId != -1)
	{
		glDeleteTextures(1, &colorId);
	}

	if (depthId != -1)
	{
		glDeleteTextures(1, &depthId);
	}

	if (stencilId != -1)
	{
		glDeleteTextures(1, &stencilId);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	if (data.bufferTypes & FrameBufferType::Color)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &colorId);
		glBindTexture(GL_TEXTURE_2D, colorId);
		glTexStorage2D(GL_TEXTURE_2D, 1, data.colorFormat, data.width, data.height);

		// Todo think about adding ability to filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Todo add ability to change Color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorId, 0);
	}

	if (data.bufferTypes & FrameBufferType::Depth && !(data.bufferTypes & FrameBufferType::Stencil))
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &depthId);
		glBindTexture(GL_TEXTURE_2D, depthId);
		glTexStorage2D(GL_TEXTURE_2D, 1, data.depthFormat, data.width, data.height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthId, 0);
	}

	if (data.bufferTypes & FrameBufferType::Stencil && !(data.bufferTypes & FrameBufferType::Depth))
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &stencilId);
		glBindTexture(GL_TEXTURE_2D, stencilId);
		glTexStorage2D(GL_TEXTURE_2D, 1, data.stencilFormat, data.width, data.height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilId, 0);
	}

	if (data.bufferTypes & FrameBufferType::Stencil && data.bufferTypes & FrameBufferType::Depth)
	{
		uint32_t dataType = 0;

		if (data.depthFormat == DepthFormat::Depth24 && data.stencilFormat == StencilFormat::Stencil8)
		{
			dataType = GL_DEPTH24_STENCIL8;
		}
		else if (data.depthFormat == DepthFormat::Depth32F && data.stencilFormat == StencilFormat::Stencil8)
		{
			dataType = GL_DEPTH32F_STENCIL8;
		}
		else
		{
			assert(!"Depth format and stencil supports only 24_8 and 32F_8");
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &depthId);
		glBindTexture(GL_TEXTURE_2D, depthId);
		glTexStorage2D(GL_TEXTURE_2D, 1, dataType, data.width, data.height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthId, 0);
	}

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "FrameBuffer cration failed!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FearEngine::Render::FrameBuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FearEngine::Render::FrameBuffer::disable() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

FearEngine::Render::FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &frameBufferId); }
