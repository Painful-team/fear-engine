#ifndef FEARENGINE_CACHE_LOADER_H__
#define FEARENGINE_CACHE_LOADER_H__

#include <memory>
#include <string_view>

#include "ErrorCodes.hpp"
#include "Resource.hpp"

namespace FearEngine::Cache
{
class Loader
{
public:
	virtual int init() = 0;
	virtual std::string getPattern() const = 0;
	virtual Cache::errorCode load(const std::string_view& filename,
		 std::shared_ptr<Resource>& resource,
		 const ResourceFlags flags = resourceFlag::None) = 0;

	virtual ~Loader() = default;
};
}  // namespace FearEngine::Cache
#endif
