#ifndef FEARENGINE_CACHE_LOADER_H__
#define FEARENGINE_CACHE_LOADER_H__

#include <memory>
#include <string_view>

#include "Resource.hpp"
#include "ErrorCodes.hpp"

namespace FearEngine::Cache
{
class Loader
{
public:
	virtual int init() = 0;
	virtual std::string getPattern() const = 0;
	virtual Cache::errorCode load(const std::string_view& filename, std::shared_ptr<Resource>& resource) = 0;

	virtual ~Loader() = default;
};
}
#endif

