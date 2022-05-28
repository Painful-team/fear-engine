#include "MaterialResource.hpp"

#include <core/Engine.hpp>


FearEngine::Cache::errorCode loadMaterialMap(const std::string& fileName, std::shared_ptr<FearEngine::Cache::Resource>& resource)
{
	if (fileName.empty())
	{
		return FearEngine::Cache::errorCodes::OK;
	};

	return FearEngine::Engine::getCache()->getResource(fileName, resource);
}

std::shared_ptr<FearEngine::Cache::Material> FearEngine::Cache::Material::create(const tinyobj_opt::material_t& material)
{
	auto materialRef = std::make_shared<Material>();

	materialRef->ambient = {material.ambient[0], material.ambient[1], material.ambient[2]};
	materialRef->diffuse = {material.diffuse[0], material.diffuse[1], material.diffuse[2]};
	materialRef->specular = {material.specular[0], material.specular[1], material.specular[2]};
	materialRef->transmittance = {material.transmittance[0], material.transmittance[1], material.transmittance[2]};
	materialRef->emission = {material.emission[0], material.emission[1], material.emission[2]};

	materialRef->shininess = material.shininess;
	materialRef->ior = material.ior;
	materialRef->dissolve = material.dissolve;
	materialRef->illum = material.illum;

	auto result = loadMaterialMap(material.ambient_texname, materialRef->ambientRes);
	assert(result == errorCodes::OK);

	result = loadMaterialMap(material.diffuse_texname, materialRef->diffuseRes);
	assert(result == errorCodes::OK);

	result = loadMaterialMap(material.specular_texname, materialRef->specularRes);
	assert(result == errorCodes::OK);

	result = loadMaterialMap(material.specular_highlight_texname, materialRef->specularHighlightRes);
	assert(result == errorCodes::OK);

	result = loadMaterialMap(material.bump_texname, materialRef->bumpRes);
	assert(result == errorCodes::OK);

	result = loadMaterialMap(material.displacement_texname, materialRef->displacementRes);
	assert(result == errorCodes::OK);

	result = loadMaterialMap(material.alpha_texname, materialRef->alphaRes);
	assert(result == errorCodes::OK);

	return materialRef;
}

FearEngine::Cache::ResourceExtraType FearEngine::Cache::Material::type() const { return ResourceExtraType::Mat; }
