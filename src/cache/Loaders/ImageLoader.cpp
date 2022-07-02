#include "ImageLoader.hpp"

#include <stb_image.h>

#include <cache/ImageResource.hpp>

#include <core/Engine.hpp>

FearEngine::Cache::Loaders::ImageLoader::ImageLoader(std::string _pattern)
 : pattern(_pattern)
{}

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::ImageLoader::init()
{
	Engine::logs()->log("Cache", "{0} loader initialization has begun.", getPattern());

	Engine::logs()->log("Cache", "{0} loader initialization has ended successfully.", getPattern());
	return Cache::errorCodes::OK;
}

std::string FearEngine::Cache::Loaders::ImageLoader::getPattern() const { return pattern; }

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::ImageLoader::load(const std::string_view& filename,
	 std::shared_ptr<Resource>& resource,
	 ResourceFlags flags)
{
	Engine::logs()->log("Cache", "Loading of {0} with flags {1} has begun.", filename, flags);

	int width;
	int height;
	int channels;

	stbi_set_flip_vertically_on_load(flags & ResourceFlag::FlipImageVertically);

	auto data = reinterpret_cast<int8_t*>(stbi_load(filename.data(), &width, &height, &channels, 0));
	if (data == nullptr)
	{
		Engine::logs()->log("Cache", "Loading of {0} with flags {1} has failed with error \"{2}\".", filename, flags, stbi_failure_reason());
		return Cache::errorCodes::IMAGE_LOAD_FAILED;
	}

	resource = std::make_shared<Resource>();
	resource->filename = filename;
	resource->data = data;
	resource->size = width * height * channels;
	resource->flags = flags;

	auto extra = std::make_shared<ImageData>();

	extra->width = width;
	extra->height = height;
	extra->channels = channels;

	resource->deleter = stbi_image_free;

	resource->extra = extra;

	Engine::logs()->log("Cache", "Loading of {0} with flags {1} has ended successfully.\nFilename:{0}\nwidth:{2}\nheigth:{3}\nchannels:{4}", filename, flags, width, height, channels);

	return Cache::errorCodes::OK;
}
