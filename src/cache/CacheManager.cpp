#include "CacheManager.hpp"

#include <cassert>

#include "ErrorCodes.hpp"

#include <utils/StringUtils.hpp>

#include "Loaders/ImageLoader.hpp"
#include "Loaders/ObjLoader.hpp"

#include <core/Engine.hpp>

FearEngine::Cache::errorCode FearEngine::CacheManager::init()
{
	Engine::logs()->log("Cache", "Cache Manager initialization has begun.");
	loaders.emplace("*.obj", new Cache::Loaders::ObjLoader);
	loaders.emplace("*.jpg", new Cache::Loaders::ImageLoader("*.jpg"));
	loaders.emplace("*.png", new Cache::Loaders::ImageLoader("*.png"));

	for (auto& pair : loaders)
	{
		auto result = pair.second->init();
		assert(result == Cache::errorCodes::OK);
	}

	Engine::logs()->log("Cache", "Cache Manager initialization has ended.");

	return Cache::errorCodes::OK;
}

FearEngine::Cache::errorCode FearEngine::CacheManager::getResource(const std::string_view& fileName,
	 std::shared_ptr<Cache::Resource>& resource,
	 Cache::ResourceFlags flags)
{
	Engine::logs()->log("Cache", "{0} with flags {1} has requested.", fileName, flags);

	if (resources.find({fileName.data(), flags}) != resources.end())
	{
		resource = resources.at({fileName.data(), flags});

		Engine::logs()->log("Cache", "{0} with flags {1} has found.", fileName, flags);

		updatePriority(resource);
		return Cache::errorCodes::OK;
	}

	for (auto& pair : loaders)
	{
		if (utils::WildcardMatch(pair.first.c_str(), fileName.data()))
		{
			auto result = pair.second->load(fileName, resource, flags);
			if (result == Cache::errorCodes::OK)
			{
				addNewResource(resource, flags);
			}

			return result;
		}
	}

	Engine::logs()->error("Cache", "No loader found for {0} with flags {1}.", fileName, flags);

	return Cache::errorCodes::LOADER_NOT_FOUND;
}

int FearEngine::CacheManager::prepare(const std::string_view& fileName, Cache::ResourceFlags flags)
{
	std::shared_ptr<Cache::Resource> resource;

	Engine::logs()->log("Cache", "{0} with flags {1} has requested to prepare.", fileName, flags);

	return getResource(fileName, resource, flags);
}

FearEngine::Cache::errorCode FearEngine::CacheManager::releaseResource(const std::string_view& fileName, Cache::ResourceFlags flags)
{
	auto resource = resources.find({fileName.data(), flags});
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
