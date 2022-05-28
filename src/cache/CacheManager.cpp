#include "CacheManager.hpp"

#include <cassert>

#include "ErrorCodes.hpp"

#include <utils/StringUtils.hpp>

#include "Loaders/ImageLoader.hpp"
#include "Loaders/ObjLoader.hpp"

FearEngine::Cache::errorCode FearEngine::CacheManager::init()
{
	loaders.emplace("*.obj", new Cache::Loaders::ObjLoader);
	loaders.emplace("*.jpg", new Cache::Loaders::ImageLoader("*.jpg"));
	loaders.emplace("*.png", new Cache::Loaders::ImageLoader("*.png"));

	for (auto& pair : loaders)
	{
		pair.second->init();
	}

	return Cache::errorCodes::OK;
}

FearEngine::Cache::errorCode FearEngine::CacheManager::getResource(const std::string_view& file_name,
	 std::shared_ptr<Cache::Resource>& resource,
	 Cache::ResourceFlags flags)
{
	if (resources.find({file_name.data(), flags}) != resources.end())
	{
		resource = resources.at({file_name.data(), flags});

		updatePriority(resource);
		return Cache::errorCodes::OK;
	}

	for (auto& pair : loaders)
	{
		if (utils::WildcardMatch(pair.first.c_str(), file_name.data()))
		{
			auto result = pair.second->load(file_name, resource, flags);
			if (result == Cache::errorCodes::OK)
			{
				addNewResource(resource, flags);
			}

			return result;
		}
	}

	return Cache::errorCodes::LOADER_NOT_FOUND;
}

int FearEngine::CacheManager::prepare(const std::string_view& file_name)
{
	std::shared_ptr<Cache::Resource> resource;

	return getResource(file_name, resource);
}

FearEngine::Cache::errorCode FearEngine::CacheManager::releaseResource(const std::string_view& file_name, Cache::ResourceFlags flags)
{
	auto resource = resources.find({file_name.data(), flags});
	if (resource == resources.end())
	{
		return Cache::errorCodes::RESOURCE_NOT_FOUND;
	}

	priority.remove(resource->second.get());
	resources.erase(resource);

	return Cache::errorCodes::OK;
}

void FearEngine::CacheManager::addLoader(const std::string_view& filter, Cache::Loader* loader)
{
	assert(loaders.find(filter.data()) == loaders.end() && "Loader mask already exists");
	assert(loader == nullptr && "Loader is nullptr");

	loaders.emplace(filter, loader);
}

FearEngine::CacheManager::~CacheManager()
{
	for (auto& pair : loaders)
	{
		delete pair.second;
	}

	clear();
}

void FearEngine::CacheManager::updatePriority(std::shared_ptr<Cache::Resource>& resource)
{
	priority.remove(resource.get());
	priority.push_front(resource.get());
}

void FearEngine::CacheManager::addNewResource(std::shared_ptr<Cache::Resource>& resource, Cache::ResourceFlags flags)
{
	resources.emplace(std::pair{resource->filename.data(), flags}, resource);
	priority.push_front(resource.get());
}

void FearEngine::CacheManager::clear()
{
	while (!resources.empty())
	{
		auto& it = resources.begin();
		free(it->second, it->first.second);
	}
}

void FearEngine::CacheManager::free(std::shared_ptr<Cache::Resource>& resource, Cache::ResourceFlags flags)
{
	priority.remove(resource.get());
	resources.erase({resource->filename.data(), flags});
}

std::size_t FearEngine::Cache::detail::__hasher::operator()(const std::pair<std::string_view, Cache::ResourceFlags>& pair) const
{ 
	return boost::hash_value(pair);
}
