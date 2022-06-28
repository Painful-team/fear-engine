#ifndef FEARENGINE_RENDER_TEXTURE_H__
#define FEARENGINE_RENDER_TEXTURE_H__

#include <cache/ImageResource.hpp>
#include <memory>

namespace FearEngine::Render
{
class Texture
{
public:
	Texture();
	~Texture();

	Texture(const Texture& tex) = delete;
	Texture(Texture&& tex) noexcept;

	Texture& operator=(const Texture& tex) = delete;
	Texture& operator=(Texture&& tex) noexcept;

	void init(std::shared_ptr<Cache::Resource>& resource);
	
	void initEmpty(uint8_t textureColor[3]);
	bool isEmpty() const;

	uint32_t getTexHandle() const;
	const std::shared_ptr<Cache::Resource>& getResource() const;

	void enable(uint8_t slot);
	void disable();

private:
	std::shared_ptr<Cache::Resource> texRes;
	uint32_t textureId;
};
}  // namespace FearEngine::Render

#endif	// !
