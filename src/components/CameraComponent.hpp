#ifndef FEARENGINE_COMPONENTS_CAMERACOMPONENT_H__
#define FEARENGINE_COMPONENTS_CAMERACOMPONENT_H__

#include <array>

#include <glm/glm.hpp>

#include <render/FrameBuffer.hpp>
#include <render/shader/Shader.hpp>

#include <core/events.hpp>

#include "Base.hpp"

#undef near
#undef far

namespace FearEngine::Component
{
struct Camera
{
public:
	Camera() = delete;
	Camera(Transform* transform,
		 Render::FrameBufferParams& params,
		 float fieldOfView = 90,
		 float near = 0.1,
		 float far = 100,
		 bool orthographic = false);

	Camera(const Camera& other) = delete;
	Camera(Camera&& other) noexcept;

	Camera& operator=(const Camera& other) = delete;
	Camera& operator=(Camera&& other) noexcept;

	void beginView();
	void end();

	void setUniforms(const Render::Shaders::Uniform& uniform, const Render::Shaders::Uniform& view);

	float getFOV() const;
	void setFOV(const float fov);

	float getNear() const;
	float getFar() const;

	void setNear(float camNearPlane);
	void setFar(float camFarPlane);

	Render::FrameBuffer& getFrameBuffer();
	void setFrameBuffer(Render::FrameBuffer& buffer);

	bool isOrthograpic();
	void setProjection(const bool orthographic);
	const glm::mat4& getProjection() const;
	glm::mat4 getView() const;

	void updateCameraPos();
	void updateUniformData();

private:
	Transform* transform;

	// Todo think about moving it out of the CameraComponent
	Render::Shaders::Uniform camera;
	Render::Shaders::Uniform viewUn;

	Render::FrameBuffer frameBuffer;

	float fov;
	float nearPlane;
	float farPlane;

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::mat4 projection;

	bool orthographic;

	static constexpr const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	friend class NoclipCameraController;
};

class NoclipCameraController
{
public:
	NoclipCameraController() = delete;
	NoclipCameraController(Camera* cam, float camSpeed = 0.1, const glm::vec2& camSensivity = {0.1, 0.1});
	NoclipCameraController(const NoclipCameraController& other) = delete;
	NoclipCameraController(NoclipCameraController&& other) noexcept;
	NoclipCameraController& operator=(const NoclipCameraController& other) = delete;
	NoclipCameraController& operator=(NoclipCameraController&& other) noexcept;
	~NoclipCameraController();

	void initEvents();
	void detachEvents();

	float getSpeed() const;
	void setSpeed(const float camSpeed);

	const glm::vec2& getSensivity() const;
	void setXSensivity(float x);
	void setYSensivity(float y);
	void setSensivity(float x, float y);

	bool onMove(Events::MouseMoved* evnt);
	bool onMousePressed(Events::MouseButtonPressed* evnt);
	bool onMouseReleased(Events::MouseButtonReleased* evnt);
	bool onMouseScrolled(Events::MouseScrolled* evnt);
	bool onKeyPressed(Events::KeyPressed* evnt);
	bool onKeyReleased(Events::KeyReleased* evnt);
	bool onKeyTyped(Events::KeyTyped* evnt);
	bool onActiveCamera(Events::ActiveViewport* evnt);

	void onResize(int width, int height);

private:
	Camera* camera;
	float speed;

	glm::vec2 sensivity;

	glm::vec2 mousePos;

	bool isInputEnabled = false;

	int flyActive = false;
	int flyInitialized = false;

	bool eventInitialized = false;

	static constexpr uint8_t EventAttached = 8; 
	std::array<std::pair<uint32_t, uint32_t>, EventAttached> attachedEventHandles;
};
}  // namespace FearEngine::Component
#endif
