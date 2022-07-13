#ifndef FEARENGINE_RENDER_LIGHTPASS_H__
#define FEARENGINE_RENDER_LIGHTPASS_H__

#include <array>

#include "Layer.hpp"

#include "TextureBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "shader/Shader.hpp"

#include <components/MaterialComponent.hpp>

namespace FearEngine::Render
{
class LightPass: public Layer
{
public:
	LightPass();
	errorCode init() override;
	void resize(int width, int height) override;
	void preUpdate(Component::Camera& cam) override;
	void update(Component::Camera& cam) override;
	void postUpdate(Component::Camera& cam) override;
	Render::debugProperty debugProperty() const override;

private:
// Todo Think about sending whole component
#pragma pack(push, 1)
	struct Light  // connected to fragmentShader unpackLight
	{
		glm::vec3 dir;
		int isPoint;

		glm::vec3 pos;
		float distance;

		glm::vec3 lightColor;
		float outerCutOff;

		float cutOff;

		Light& operator=(const Component::Light& light);
	};
#pragma pack(pop)

	static constexpr const uint16_t maxLightCount = 1000;

	// Todo add ability of shader read to modify constant inside shader files
	static constexpr const uint16_t maxDirlightCount = 100;

	Render::TextureBuffer lightData;
	std::array<Light, maxLightCount> lights;
	Render::Shaders::Uniform lightCountUniform;

	Render::Shaders::Shader shader;
	Render::VertexArray arr;

	Render::Shaders::Uniform dir;
	Render::Shaders::Uniform lightColor;
	Render::Shaders::Uniform dirLightCount;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;
};
};	// namespace FearEngine::Render
#endif
