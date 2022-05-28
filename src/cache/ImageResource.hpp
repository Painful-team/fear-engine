#ifndef FEARENGINE_CACHE_IMAGERESOURCE_H__
#define FEARENGINE_CACHE_IMAGERESOURCE_H__

#include <tinyobj_loader_opt.h>

#include <Cache/MaterialResource.hpp>
#include <Cache/Resource.hpp>

#include <vector>

namespace FearEngine::Cache
{
class ImageData: public ResourceExtra
{
public:
	int width;
	int height;
	int channels;

	GENRESOURCECLASSESSETIALS(Image);
};
}  // namespace FearEngine::Cache

#endif
