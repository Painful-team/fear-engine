#ifndef FEARENGINE_RENDER_FRAMEBUFFER_H__
#define FEARENGINE_RENDER_FRAMEBUFFER_H__

#include <stdint.h>
#include <unordered_map>

#include <glad/glad.h>

#include <core/Window.hpp>

#include <glm/glm.hpp>

namespace FearEngine::Render
{
using FrameBufferTypes = short;
namespace FrameBufferType
{
enum : short
{
	None = 0,
	Color = 1 << 0,
	Depth = 1 << 1,
	Stencil = 1 << 2,
	Additional = 1 << 3
};
}

using ColorFormats = uint32_t;
namespace ColorFormat
{
enum : ColorFormats
{
	None = 0,
	RGBA8 = GL_RGBA8,
	RGBA16 = GL_RGBA16,
	R32 = GL_R32I,
	RGBA32F = GL_RGBA32F,
	RGB8 = GL_RGB8,
	RGB16 = GL_RGB16,
	RGB32F = GL_RGB32F,
	R8 = GL_R8,
	R16 = GL_R16,
	R32F = GL_R32F,
	RG8 = GL_RG8,
	RG16 = GL_RG16,
	RG32F = GL_RG32F
};
}

using DepthFormats = uint32_t;
namespace DepthFormat
{
enum : DepthFormats
{
	None = 0,
	Depth16 = GL_DEPTH_COMPONENT16,
	Depth24 = GL_DEPTH_COMPONENT24,
	Depth32 = GL_DEPTH_COMPONENT32,
	Depth32F = GL_DEPTH_COMPONENT32F
};
}

using StencilFormats = uint32_t;
namespace StencilFormat
{
enum : StencilFormats
{
	None = 0,
	Stencil1 = GL_STENCIL_INDEX1,
	Stencil4 = GL_STENCIL_INDEX4,
	Stencil8 = GL_STENCIL_INDEX8,
	Stencil16 = GL_STENCIL_INDEX16
};
}

struct FrameBufferParams
{
	uint32_t width = Window::defaultWidth;
	uint32_t height = Window::defaultHeight;

	FrameBufferTypes bufferTypes;

	ColorFormats colorFormat;
	DepthFormats depthFormat;
	StencilFormats stencilFormat;

	ColorFormats additionalBufferFormat;
};

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(const FrameBuffer& buffer) = delete;
	FrameBuffer(FrameBuffer&& buffer) noexcept;
	FrameBuffer& operator=(const FrameBuffer& other) = delete;
	FrameBuffer& operator=(FrameBuffer&& other) noexcept;

	glm::vec4 getPixel(FrameBufferTypes type, glm::vec2& pos);

	void init(const FrameBufferParams& params);
	uint32_t getColorAttachment() const;
	uint32_t getDepthAttachment() const;
	uint32_t getStencilAttachment() const;
	uint32_t getAdditionalAttachment() const;

	const FrameBufferParams& getParams() const;
	void setParams(const FrameBufferParams& params);

	void onResize();

	void enable();
	void disable();

	bool isInitialized() const;

	~FrameBuffer();

private:
	FrameBufferParams data;

	uint32_t colorId;
	uint32_t depthId;
	uint32_t stencilId;
	uint32_t additionalId;

	uint32_t frameBufferId;

	FrameBufferTypes enabled;

	int initialized;
};
}  // namespace FearEngine::Renderer

#endif
