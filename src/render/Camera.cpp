#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Engine.hpp>

#include <event/MouseEvent.hpp>

#include <iostream>

namespace FearEngine::Render
{
Camera::Camera() {}

int Camera::init(FrameBufferParams& bufParams,
	 const glm::vec3& pos,
	 const glm::vec3& ang,
	 const float fieldOfView,
	 const float cameraSpeed,
	 const glm::vec2& cameraSensivity,
	 const bool isOrthographic)
{
	cameraPos = pos;
	speed = cameraSpeed;
	sensivity = cameraSensivity;
	orthographic = isOrthographic;
	fov = fieldOfView;
	frameBuffer.init(bufParams);

	setAngle(ang.x, ang.y, ang.z);

	Engine::getDispatcher()->get<Events::MouseMoved>()->attach<&Camera::onMove>(this);
	Engine::getDispatcher()->get<Events::MouseButtonPressed>()->attach<&Camera::onMousePressed>(this);
	Engine::getDispatcher()->get<Events::MouseButtonReleased>()->attach<&Camera::onMouseReleased>(this);
	Engine::getDispatcher()->get<Events::MouseScrolled>()->attach<&Camera::onMouseScrolled>(this);
	Engine::getDispatcher()->get<Events::KeyPressed>()->attach<&Camera::onKeyPressed>(this);
	Engine::getDispatcher()->get<Events::KeyReleased>()->attach<&Camera::onKeyReleased>(this);
	Engine::getDispatcher()->get<Events::KeyTyped>()->attach<&Camera::onKeyTyped>(this);

	return 0;
}

void Camera::beginView()
{
	frameBuffer.enable();
	updatePos();
}

void Camera::end() { frameBuffer.disable(); }

void Camera::setUniforms(const Shaders::Uniform& uniform, const Shaders::Uniform& view)
{
	camera = uniform;
	viewUn = view;
	setFOV(fov);
	setAngle(angles.x, angles.y, angles.z);
}

float Camera::getSpeed() const { return speed; }

void Camera::setSpeed(const float speed) { this->speed = speed; }

float Camera::getFOV() const { return fov; }

void Camera::setFOV(const float fove)
{
	auto params = frameBuffer.getParams();
	if (orthographic)
	{
		auto aspectRation = (float)params.width / (float)params.height;
		auto zoom = 0.25f;
		projection = glm::ortho(-aspectRation * zoom, aspectRation * zoom, -zoom, zoom, -1.0f, 1.0f);
	}
	else
	{
		projection = glm::perspective(glm::radians(fove), (float)params.width / (float)params.height, 0.1f, 100.0f);
	}

	fov = fove;
}

void Camera::setPos(float x, float y, float z) { cameraPos = {x, y, z}; }

const glm::vec3& Camera::getAngle() const { return angles; }

void Camera::setAngle(float pitch, float yaw, float roll)
{
	angles.x = pitch;
	angles.y = yaw;
	angles.z = roll;

	glm::vec3 front;
	front.x = cos(glm::radians(angles.y)) * cos(glm::radians(angles.x));
	front.y = sin(glm::radians(angles.x));
	front.z = sin(glm::radians(angles.y)) * cos(glm::radians(angles.x));
	cameraFront = glm::normalize(front);
}

const glm::vec3& Camera::getPos() const { return cameraPos; }

const glm::vec2& Camera::getSensivity() const { return sensivity; }

FrameBuffer& Camera::getFrameBuffer() { return frameBuffer; }

void Camera::setFrameBuffer(FrameBuffer& buffer) { frameBuffer = buffer; }

bool Camera::isOrthograpic() { return orthographic; }

void Camera::setProjection(const bool orthographic)
{
	this->orthographic = orthographic;

	setFOV(fov);
}

bool Camera::onMove(FearEngine::Events::MouseMoved* evnt)
{
	if (!flyActive)
	{
		return false;
	}

	float x = evnt->getX();
	float y = evnt->getY();
	if (!flyInitialized)
	{
		mousePos.x = x;
		mousePos.y = y;
		flyInitialized = true;
	}

	float xOffset = x - mousePos.x;
	float yOffset = mousePos.y - y;
	mousePos.x = x;
	mousePos.y = y;

	xOffset *= sensivity.x;
	yOffset *= sensivity.y;

	angles.y += xOffset;
	angles.x += yOffset;

	if (angles.x > 89.0f)
	{
		angles.x = 89.0f;
	}
	else if (angles.x < -89.0f)
	{
		angles.x = -89.0f;
	}

	setAngle(angles.x, angles.y, angles.z);
	return false;
}

bool Camera::onMousePressed(Events::MouseButtonPressed* evnt)
{
	if (evnt->getButton() == Events::Mouse::BUTTON_RIGHT)
	{
		Engine::getWindow()->blockCursor();
		flyActive = true;

		auto evnt = Events::MouseRequired(true);
		Engine::getDispatcher()->notify(&evnt);
	}

	return false;
}

bool Camera::onMouseReleased(Events::MouseButtonReleased* evnt)
{
	if (evnt->getButton() == Events::Mouse::BUTTON_RIGHT)
	{
		Engine::getWindow()->unblockCursor();

		flyActive = false;
		flyInitialized = false;

		auto evnt = Events::MouseRequired(false);
		Engine::getDispatcher()->notify(&evnt);
	}

	return false;
}

bool Camera::onMouseScrolled(Events::MouseScrolled* evnt) { return false; }

bool Camera::onKeyPressed(Events::KeyPressed* evnt) { return false; }

bool Camera::onKeyReleased(Events::KeyReleased* evnt) { return false; }

// Todo add timebased movement to be able to stop relying on frames
bool Camera::onKeyTyped(Events::KeyTyped* evnt)
{
	bool moved = false;
	// Todo add ability to remap keys
	if (evnt->keyCode() == Events::Key::W)
	{
		cameraPos += speed * cameraFront;
	}

	if (evnt->keyCode() == Events::Key::S)
	{
		cameraPos -= speed * cameraFront;
	}

	if (evnt->keyCode() == Events::Key::A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	if (evnt->keyCode() == Events::Key::D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	return false;
}

void Camera::setXSensivity(float x) { sensivity.x = x; }

void Camera::setYSensivity(float y) { sensivity.y = y; }

void Camera::setSensivity(float x, float y)
{
	sensivity.x = x;
	sensivity.y = y;
}

void Camera::onResize(int width, int height)
{
	if (width > 1 && height > 1)
	{
		auto params = frameBuffer.getParams();
		params.width = width;
		params.height = height;

		frameBuffer.setParams(params);
		setFOV(fov);
	}
}

void Camera::updatePos()
{
	viewUn.setMat4(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	camera.setMat4(projection);
}
}  // namespace FearEngine::Render
