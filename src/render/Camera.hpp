#ifndef FEARENGINE_RENDER_CAMERA_H__
#define FEARENGINE_RENDER_CAMERA_H__

#include "shader/Uniform.hpp"

#include <core/events.hpp>

namespace FearEngine::Render
{
class Camera
{
public:
	Camera(const Shaders::Uniform& projection,
		 const Shaders::Uniform& view,
		 const glm::vec3& pos = {0, 0, 0},
		 const glm::vec3& ang = {0, -90.0, 0},
		 const float fieldOfView = 90,
		 const float cameraSpeed = 0.1,
		 const glm::vec2& cameraSensivity = {0.1, 0.1},
		 const bool orthographic = false);
	Camera(const glm::vec3& pos = {0, 0, 0},
		 const glm::vec3& ang = {0, -90.0, 0},
		 const float fieldOfView = 90,
		 const float cameraSpeed = 0.1,
		 const glm::vec2& cameraSensivity = {0.1, 0.1},
		 const bool orthographic = false);

	void setUniforms(const Shaders::Uniform& uniform, const Shaders::Uniform& view);

	float getSpeed() const;
	void setSpeed(const float speed);

	float getFOV() const;
	void setFOV(const float fov);

	const glm::vec3& getPos() const;
	void setPos(float x, float y, float z);

	const glm::vec3& getAngle() const;
	void setAngle(float pitch, float yaw, float roll);

	const glm::vec2& getSensivity() const;
	void setXSensivity(float x);
	void setYSensivity(float y);
	void setSensivity(float x, float y);

	bool isOrthograpic();
	void setProjection(const bool orthographic);

	bool onMove(Events::MouseMoved* evnt);
	bool onMousePressed(Events::MouseButtonPressed* evnt);
	bool onMouseReleased(Events::MouseButtonReleased* evnt);
	bool onMouseScrolled(Events::MouseScrolled* evnt);
	bool onKeyPressed(Events::KeyPressed* evnt);
	bool onKeyReleased(Events::KeyReleased* evnt);
	bool onKeyTyped(Events::KeyTyped* evnt);

	void updatePos();
	void init();
private:

private:
	static constexpr const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Shaders::Uniform camera;
	Shaders::Uniform viewUn;

	float fov;
	float speed;

	glm::vec2 sensivity;

	glm::vec2 mousePos;

	int flyActive = false;
	int flyInitialized = false;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 angles;

	bool orthographic;
};
}  // namespace FearEngine::Render
#endif
