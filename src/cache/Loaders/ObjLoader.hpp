#ifndef FEARENGINE_CACHE_LOADERS_OBJLOADER_H__
#define FEARENGINE_CACHE_LOADERS_OBJLOADER_H__

#include <Cache/Loader.hpp>

#include <glm/glm.hpp>

namespace FearEngine::Cache::Loaders
{
class ObjLoader: public Loader
{
public:
	errorCode init() override final;
	std::string getPattern() const override final;
	Cache::errorCode load(const std::string_view& filename,
		 std::shared_ptr<Resource>& resource,
		 ResourceFlags flags = resourceFlag::None) override final;

	static const uint8_t Threads = 2;
	~ObjLoader() override final;
};
}  // namespace FearEngine::Cache::Loaders
#endif
