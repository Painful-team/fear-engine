#ifndef FEARENGINE_CACHE_ERRORCODES_H__
#define FEARENGINE_CACHE_ERRORCODES_H__

namespace FearEngine::Cache
{
using errorCode = short;
namespace errorCodes
{
enum : errorCode
{
	OK = 0,
	LOADER_NOT_FOUND = 1,
	RESOURCE_NOT_FOUND = 2,
	EMPTY_FILE = 3,
	IMAGE_LOAD_FAILED = 4,
};
}
}  // namespace FearEngine::Cache

#endif
