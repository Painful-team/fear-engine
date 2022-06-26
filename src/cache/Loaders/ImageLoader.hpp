#ifndef FEARENGINE_CACHE_LOADERS_TEXTURELOADER_H__
#define FEARENGINE_CACHE_LOADERS_TEXTURELOADER_H__

#include <cache/ErrorCodes.hpp>
#include <cache/Loader.hpp>
#include <cache/Resource.hpp>


namespace FearEngine::Cache::Loaders
{
class ImageLoader: public Loader
{
public:
	ImageLoader(const std::string pattern);
	errorCode init() override final;
	std::string getPattern() const override final;
	Cache::errorCode load(const std::string_view& filename,
		 std::shared_ptr<Resource>& resource,
		 ResourceFlags flags = ResourceFlag::None) override final;

	~ImageLoader() override final{};

private:
	std::string pattern;
};
}  // namespace FearEngine::Cache::Loaders

#endif
