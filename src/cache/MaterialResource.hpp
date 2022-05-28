#ifndef FEARENGINE_CACHE_LOADERS_MATERIALRESOURCE_H__
#define FEARENGINE_CACHE_LOADERS_MATERIALRESOURCE_H__

#include <string>
#include "Resource.hpp"

#include <tinyobj_loader_opt.h>
#include <glm/glm.hpp>

namespace FearEngine::Cache
{
class Material: public ResourceExtra
{
public:
	static std::shared_ptr<Material> create(const tinyobj_opt::material_t& material);

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 transmittance;
	glm::vec3 emission;

	float shininess;
	float ior;
	float dissolve;
	int illum;

	std::shared_ptr<Resource> ambientRes;
	std::shared_ptr<Resource> diffuseRes;
	std::shared_ptr<Resource> specularRes;
	std::shared_ptr<Resource> specularHighlightRes;
	std::shared_ptr<Resource> bumpRes;
	std::shared_ptr<Resource> displacementRes;
	std::shared_ptr<Resource> alphaRes;

#ifdef __PBR__	// Left for future material system updates
	// PBR
	float metallic;
	float sheen;
	float clearcoatThickness;
	float clearcoatRoughness;
	float anisotropy;
	float anisotropyRotation;
	std::shared_ptr<Resource> roughnessRes;
	std::shared_ptr<Resource> metallicRes;
	std::shared_ptr<Resource> sheenRes;
	std::shared_ptr<Resource> emissiveRes;
	std::shared_ptr<Resource> normalRes;
#endif

	ResourceExtraType type() const;
};
}  // namespace FearEngine::Cache
#endif