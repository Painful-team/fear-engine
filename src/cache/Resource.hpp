#ifndef FEARENGINE_CACHE_RESOURCE_H__
#define FEARENGINE_CACHE_RESOURCE_H__

#include <memory>
#include <string_view>

#include "LoadersFlags.hpp"

namespace FearEngine::Cache
{

using ResourceFlags = uint64_t;

enum ResourceExtraType
{
	OBJ = 0,
	Mat = 1,
	Image = 2,
	invalid,
};

class ResourceExtra
{
public:
	virtual ResourceExtraType type() const = 0;
};

void deleterFunc(void* ptr);

struct Resource
{
	ResourceFlags flags;

	std::shared_ptr<ResourceExtra> extra;

	std::string filename;
	int8_t* data;
	uint32_t size;

	decltype(&deleterFunc) deleter = deleterFunc;

	~Resource();
};

#define GENRESOURCECLASSESSETIALS(classType)                                       \
	static ResourceExtraType staticType() { return ResourceExtraType::classType; } \
                                                                                   \
	ResourceExtraType type() const override { return staticType(); }
}  // namespace FearEngine::Cache

#endif
