
#include <string_view>
#include <list>
#include <unordered_map>
#include <memory>

#include "Loader.hpp"
#include "Resource.hpp"


namespace FearEngine
{
class CacheManager
{
public:
	Cache::errorCode init();
	Cache::errorCode getResource(const std::string_view& file_name, std::shared_ptr<Cache::Resource>& resource);
	int prepare(const std::string_view& file_name);

	Cache::errorCode releaseResource(const std::string_view& file_name);
	void clear();

	void addLoader(const std::string_view& filter, Cache::Loader* loader);

	~CacheManager();
private:
	void updatePriority(std::shared_ptr<Cache::Resource>& resource);
	void addNewResource(std::shared_ptr<Cache::Resource>& resource);
	void free(std::shared_ptr<Cache::Resource>& resource);

	std::list<Cache::Resource*> priority;
	std::unordered_map<std::string_view, std::shared_ptr<Cache::Resource>> resources;
	std::unordered_map<std::string, Cache::Loader*> loaders;
};
}
