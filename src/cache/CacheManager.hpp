
#include <list>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "Loader.hpp"
#include "Resource.hpp"

#include <boost/hash_combine.hpp>

namespace FearEngine
{
namespace Cache::detail
{
struct __hasher
{
	std::size_t operator()(const std::pair<std::string_view, Cache::ResourceFlags>&) const;
};
}  // namespace detail

class CacheManager
{
public:
	Cache::errorCode init();
	Cache::errorCode getResource(const std::string_view& file_name,
		 std::shared_ptr<Cache::Resource>& resource,
		 Cache::ResourceFlags flags = Cache::resourceFlag::None);
	int prepare(const std::string_view& file_name);

	Cache::errorCode releaseResource(const std::string_view& file_name, Cache::ResourceFlags flags);
	void clear();

	void addLoader(const std::string_view& filter, Cache::Loader* loader);

	~CacheManager();

private:
	void updatePriority(std::shared_ptr<Cache::Resource>& resource);
	void addNewResource(std::shared_ptr<Cache::Resource>& resource, Cache::ResourceFlags flags);
	void free(std::shared_ptr<Cache::Resource>& resource, Cache::ResourceFlags flags);

	std::list<Cache::Resource*> priority;
	std::unordered_map<std::pair<std::string_view, Cache::ResourceFlags>, std::shared_ptr<Cache::Resource>, Cache::detail::__hasher>
		 resources;
	std::unordered_map<std::string, Cache::Loader*> loaders;
};
}  // namespace FearEngine
