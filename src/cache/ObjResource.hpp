#ifndef FEARENGINE_CACHE_LOADERS_OBJRESOURCE_H__
#define FEARENGINE_CACHE_LOADERS_OBJRESOURCE_H__

#include <tinyobj_loader_opt.h>

#include <Cache/Resource.hpp>

#include <vector>

namespace FearEngine::Cache
{
class ObjData : public ResourceExtra
{
public:
	std::vector<float> vertices;
	std::vector<tinyobj_opt::material_t> materials;

	GENRESOURCECLASSESSETIALS(OBJ);
};
}

#endif
