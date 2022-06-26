#ifndef FEARENGINE_CACHE_LOADERSFLAGS_H__
#define FEARENGINE_CACHE_LOADERSFLAGS_H__

namespace FearEngine::Cache
{

using ResourceFlags = uint64_t;

namespace ResourceFlag
{
enum : ResourceFlags
{
	None = 0,
	FlipImageVertically = 1 << 0,
	RecalcNormals = 1 << 1,
};
}
}  // namespace FearEngine::Cache

#endif
