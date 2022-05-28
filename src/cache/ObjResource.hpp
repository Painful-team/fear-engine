#ifndef FEARENGINE_CACHE_OBJRESOURCE_H__
#define FEARENGINE_CACHE_OBJRESOURCE_H__

#include <tinyobj_loader_opt.h>

#include <Cache/MaterialResource.hpp>
#include <Cache/Resource.hpp>

#include <vector>

namespace FearEngine::Cache
{
class ObjData: public ResourceExtra
{
public:
	std::vector<float> vertices;
	std::vector<std::shared_ptr<Material>> materials;

	GENRESOURCECLASSESSETIALS(OBJ);
};
}  // namespace FearEngine::Cache

#endif
