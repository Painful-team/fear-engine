#ifndef FEARENGINE_CACHE_RESOURCE_H__
#define FEARENGINE_CACHE_RESOURCE_H__

#include <string_view>
#include <memory>
namespace FearEngine::Cache
{

enum ResourceExtraType
{
	OBJ = 0,

	invalid,
};

class ResourceExtra
{
public:
	virtual ResourceExtraType type() const = 0;
};

struct Resource
{
	std::shared_ptr<ResourceExtra> extra;

	std::string filename;
	char* data;
	uint32_t size;
};

#define GENRESOURCECLASSESSETIALS(classType) 	\
static ResourceExtraType staticType()					\
{												\
	return ResourceExtraType::classType;				\
}												\
												\
ResourceExtraType type() const override						\
{												\
	return staticType();						\
}												
}

#endif
