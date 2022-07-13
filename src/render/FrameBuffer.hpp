#ifndef FEARENGINE_RENDER_FRAMEBUFFER_H__
#define FEARENGINE_RENDER_FRAMEBUFFER_H__

#include <array>
#include <unordered_map>

#include <glad/glad.h>

#include <core/Window.hpp>

#include <glm/glm.hpp>

namespace FearEngine::Render
{
using FrameBufferTypes = uint64_t;
namespace FrameBufferType
{
enum : FrameBufferTypes
{
	None = 0,
	Depth = 1 << 0,
	Stencil = 1 << 1,
	ColorAttachment0 = 1 << 2,
	ColorAttachment1 = 1 << 3,
	ColorAttachment2 = 1 << 4,
	ColorAttachment3 = 1 << 5,
	ColorAttachment4 = 1 << 6,
	ColorAttachment5 = 1 << 7,
	ColorAttachment6 = 1 << 8,
	ColorAttachment7 = 1 << 9,
	ColorAttachment8 = 1 << 10,
	ColorAttachment9 = 1 << 11,
	ColorAttachment10 = 1 << 12,
	ColorAttachment11 = 1 << 13,
	ColorAttachment12 = 1 << 14,
	ColorAttachment13 = 1 << 15,
	ColorAttachment14 = 1 << 16,
	ColorAttachment15 = 1 << 17,
	ColorAttachment16 = 1 << 18,
	ColorAttachment17 = 1 << 19,
	ColorAttachment18 = 1 << 20,
	ColorAttachment19 = 1 << 21,
	ColorAttachment20 = 1 << 22
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
	static constexpr const uint32_t maxColorAttachments = 8;

	uint32_t width = Window::defaultWidth;
	uint32_t height = Window::defaultHeight;

	FrameBufferTypes bufferTypes;

	std::array<ColorFormats, maxColorAttachments> colorFormat;
	std::array<float, maxColorAttachments> clearValues;
	DepthFormats depthFormat;
	StencilFormats stencilFormat;
};

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(const FrameBuffer& buffer) = delete;
	FrameBuffer(FrameBuffer&& buffer) noexcept;
	FrameBuffer& operator=(const FrameBuffer& other) = delete;
	FrameBuffer& operator=(FrameBuffer&& other) noexcept;

	// Supports only one buffer at the time
	glm::vec4 getPixel(FrameBufferTypes type, glm::vec2& pos);

	void init(const FrameBufferParams& params);
	uint32_t getColorAttachment(uint8_t attachNum) const;
	void bindColorAttachment(uint8_t attachNum, uint8_t textureUnit);
	uint32_t getDepthAttachment() const;
	uint32_t getStencilAttachment() const;

	const FrameBufferParams& getParams() const;
	void setParams(const FrameBufferParams& params);

	void onResize();

	void enable(uint32_t* colorAttachmentNum = nullptr, uint8_t size = 0, bool disableDepth = false, bool clear = false);
	void disable();
	void clear();

	bool isInitialized() const;

	~FrameBuffer();

private:
	FrameBufferParams data;

	static constexpr const float defaultClear = 0;

	std::array<uint32_t, FrameBufferParams::maxColorAttachments> colors;

	uint32_t depthId;
	uint32_t stencilId;

	uint32_t frameBufferId;

	FrameBufferTypes enabled;

	int initialized;

	friend struct FrameBufferParams;
};
}  // namespace FearEngine::Render

#endif
