#ifndef FEARENGINE_RENDER_BUFFER_H__
#define FEARENGINE_RENDER_BUFFER_H__

#include <cstdint>
namespace FearEngine::Render
{
class TextureBuffer
{
public:
	TextureBuffer();

	void genBuffer();
	void bind(uint16_t unit) const;

	void reserveData(const uint32_t size);
	void setData(void* data, const uint32_t size);

	void unbind() const;

	~TextureBuffer();

private:
	uint32_t buffer;
	uint32_t texture;

	int8_t* data;
};
};	// namespace FearEngine::Render
#endif
