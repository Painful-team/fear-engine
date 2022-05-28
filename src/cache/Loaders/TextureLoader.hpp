#ifndef FEARENGINE_CACHE_LOADERS_TEXTURELOADER_H__
#define FEARENGINE_CACHE_LOADERS_TEXTURELOADER_H__

#include <cache/Loader.hpp>
#include <cache/ErrorCodes.hpp>
#include <cache/Resource.hpp>

namespace FearEngine::Cache::Loaders
{
class TextureLoader : public Loader
{
	int init() override final;
	std::string getPattern() const override final;
	Cache::errorCode load(const std::string_view& filename, std::shared_ptr<Resource>& resource) override final;

	static const uint8_t Threads = 2;
	~TextureLoader() override final;
};
}

#endif
