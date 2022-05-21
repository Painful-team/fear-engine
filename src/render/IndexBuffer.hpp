#ifndef FEARENGINE_RENDER_INDEXBUFFER_H__
#define FEARENGINE_RENDER_INDEXBUFFER_H__

namespace FearEngine::Render
{
class IndexBuffer
{
	using uint32_t = unsigned;
public:
	IndexBuffer();

	void genBuffer();

	void bindData(const uint32_t size);
	void bindData(uint32_t* indices, const uint32_t size);
	void setData(uint32_t* indices, const uint32_t size);

	~IndexBuffer();
private:
	uint32_t EBO;
};
}

#endif
