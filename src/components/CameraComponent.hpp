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
class Camera
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

	void beginView(uint32_t* skipAttachments = nullptr, uint32_t count = 0);
	void end();

	float getFOV() const;
	void setFOV(const float fov);

	float getNear() const;
	float getFar() const;

	float getAspect() const;

	void setNear(float camNearPlane);
	void setFar(float camFarPlane);

	Render::FrameBuffer& getFrameBuffer();
	// Todo Think if that function is necessary.
	// void setFrameBuffer(Render::FrameBuffer& buffer);

	bool isOrthograpic();
	void setProjection(const bool orthographic);
	const glm::mat4& getProjection() const;
	glm::mat4 getView() const;

	void updateCameraPos();

	void onResize(int width, int height);

	GENCOMPONENTESSENTIALS(camera)
private:
	Transform* transform;

	Render::FrameBuffer frameBuffer;

	float fov;
	float nearPlane;
	float farPlane;
	float aspect;

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::mat4 projection;

	bool orthographic;

	static constexpr const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	friend class EditorCamera;
};

class EditorCamera
{
public:
	EditorCamera() = delete;
	EditorCamera(Camera* cam, float camSpeed = 0.1, const glm::vec2& camSensivity = {0.1, 0.1});
	EditorCamera(const EditorCamera& other) = delete;
	EditorCamera(EditorCamera&& other) noexcept;
	EditorCamera& operator=(const EditorCamera& other) = delete;
	EditorCamera& operator=(EditorCamera&& other) noexcept;
	~EditorCamera();

	void initEvents();
	void detachEvents();

	bool onMove(Events::MouseMoved* evnt);
	bool onMousePressed(Events::MouseButtonPressed* evnt);
	bool onMouseReleased(Events::MouseButtonReleased* evnt);
	bool onMouseScrolled(Events::MouseScrolled* evnt);
	bool onKeyPressed(Events::KeyPressed* evnt);
	bool onKeyReleased(Events::KeyReleased* evnt);
	bool onKeyTyped(Events::KeyTyped* evnt);
	bool onActiveCamera(Events::ActiveViewport* evnt);

	void onResize(int width, int height);

	GENCOMPONENTESSENTIALS(editorCamera)

	float speed;
	glm::vec2 sensivity;

private:
	Camera* camera;

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
