#ifndef FEARENGINE_CACHE_OBJRESOURCE_H__
#define FEARENGINE_CACHE_OBJRESOURCE_H__

#include <tinyobj_loader_opt.h>

#include <Cache/MaterialResource.hpp>
#include <Cache/Resource.hpp>

#include <vector>

namespace FearEngine::Cache
{
struct ObjData: public ResourceExtra
{
	static constexpr const uint8_t VertexSize = 11 * sizeof(float);
	std::vector<float> vertices;
	uint32_t count;

	std::vector<std::shared_ptr<Material>> materials;

	GENRESOURCECLASSESSETIALS(OBJ);
};
}  // namespace FearEngine::Cache

#endif
