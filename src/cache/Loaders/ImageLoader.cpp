#include "ImageLoader.hpp"

#include <stb_image.h>

#include <cache/ImageResource.hpp>

FearEngine::Cache::Loaders::ImageLoader::ImageLoader(std::string _pattern)
 : pattern(_pattern)
{}

int FearEngine::Cache::Loaders::ImageLoader::init() { return Cache::errorCodes::OK; }

std::string FearEngine::Cache::Loaders::ImageLoader::getPattern() const { return pattern; }

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::ImageLoader::load(const std::string_view& filename,
	 std::shared_ptr<Resource>& resource,
	 ResourceFlags flags)
{
	int width;
	int height;
	int channels;

	stbi_set_flip_vertically_on_load(flags & resourceFlag::FlipImageVertically);

	// TODO Write error handler
	auto data = reinterpret_cast<int8_t*>(stbi_load(filename.data(), &width, &height, &channels, 0));
	if (data == nullptr)
	{
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

	return Cache::errorCodes::OK;
}
