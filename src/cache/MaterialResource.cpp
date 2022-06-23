#include "MaterialResource.hpp"

#include <core/Logger.hpp>
#include <core/Engine.hpp>
#include <cache/CacheManager.hpp>

FearEngine::Cache::errorCode loadMaterialMap(const std::string& fileName, std::shared_ptr<FearEngine::Cache::Resource>& resource)
{
	if (fileName.empty())
	{
		return FearEngine::Cache::errorCodes::OK;
	};

	return FearEngine::Engine::getCache()->getResource(fileName, resource);
}


FearEngine::Cache::errorCode FearEngine::Cache::Material::create(const tinyobj_opt::material_t& material,
	 std::shared_ptr<FearEngine::Cache::Material>& materialRef)
{
	errorCode createResultCode = errorCodes::OK;

	Engine::logs()->log("Cache", "Load of {0} material has began.", material.name);
	Material materialObj;
	materialObj.ambient = {material.ambient[0], material.ambient[1], material.ambient[2]};
	materialObj.diffuse = {material.diffuse[0], material.diffuse[1], material.diffuse[2]};
	materialObj.specular = {material.specular[0], material.specular[1], material.specular[2]};
	materialObj.transmittance = {material.transmittance[0], material.transmittance[1], material.transmittance[2]};
	materialObj.emission = {material.emission[0], material.emission[1], material.emission[2]};

	materialObj.shininess = material.shininess;
	materialObj.ior = material.ior;
	materialObj.dissolve = material.dissolve;
	materialObj.illum = material.illum;

	auto result = loadMaterialMap(material.ambient_texname, materialObj.ambientRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} ambient map.", material.name);
		createResultCode = errorCodes::AMBIENT_LOAD_FAILED;
	}

	result = loadMaterialMap(material.diffuse_texname, materialObj.diffuseRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} diffuse map.", material.name);
		createResultCode = errorCodes::DIFFUSE_LOAD_FAILED;
	}

	result = loadMaterialMap(material.specular_texname, materialObj.specularRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} specular map.", material.name);
		createResultCode = errorCodes::SPECULAR_LOAD_FAILED;
	}

	result = loadMaterialMap(material.specular_highlight_texname, materialObj.specularHighlightRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} specular highlight map.", material.name);
		createResultCode = errorCodes::SPECULAR_HIGHLIGHTS_LOAD_FAILED;
	}

	result = loadMaterialMap(material.bump_texname, materialObj.bumpRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} bump map.", material.name);
		createResultCode = errorCodes::BUMP_LOAD_FAILED;
	}

	result = loadMaterialMap(material.displacement_texname, materialObj.displacementRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} displacement map.", material.name);
		createResultCode = errorCodes::DISPLACEMENT_LOAD_FAILED;
	}

	result = loadMaterialMap(material.alpha_texname, materialObj.alphaRes);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Failed to load material {0} alpha map.", material.name);
		createResultCode = errorCodes::ALPHA_LOAD_FAILED;
	}

	materialRef = std::make_shared<Material>();
	*materialRef = std::move(materialObj);

	if (createResultCode == errorCodes::OK)
	{
		Engine::logs()->log("Cache", "{0} has loaded successfully.", material.name);
	}
	else
	{
		Engine::logs()->error("Cache", "{0} has loaded with map errors.", material.name);
	}

	return createResultCode;
}

FearEngine::Cache::Material::Material(Material&& other) noexcept { *this = std::move(other); }

FearEngine::Cache::Material& FearEngine::Cache::Material::operator=(Material&& other) noexcept
{
	ambient = other.ambient;
	diffuse = other.diffuse;
	specular = other.specular;
	transmittance = other.transmittance;
	emission = other.emission;

	shininess = other.shininess;
	ior = other.ior;
	dissolve = other.dissolve;
	illum = other.illum;

	ambientRes = std::move(other.ambientRes);
	diffuseRes = std::move(other.diffuseRes);
	specularRes = std::move(other.specularRes);
	specularHighlightRes = std::move(other.specularHighlightRes);
	bumpRes = std::move(other.bumpRes);
	displacementRes = std::move(other.displacementRes);
	alphaRes = std::move(other.alphaRes);

	return *this;
}

FearEngine::Cache::ResourceExtraType FearEngine::Cache::Material::type() const { return ResourceExtraType::Mat; }
