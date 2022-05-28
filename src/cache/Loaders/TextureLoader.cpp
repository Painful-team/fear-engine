#include "TextureLoader.hpp"

int FearEngine::Cache::Loaders::TextureLoader::init()
{
	return 0;
}

std::string FearEngine::Cache::Loaders::TextureLoader::getPattern() const
{
	return std::string();
}

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::TextureLoader::load(const std::string_view& filename, std::shared_ptr<Resource>& resource)
{
	return Cache::errorCode();
}

FearEngine::Cache::Loaders::TextureLoader::~TextureLoader()
{}
