#include "Camera.hpp"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <core/Engine.hpp>

namespace FearEngine::Render
{
	Camera::Camera(const float fov, const float speed, const bool orthographic):
		speed(speed),
		orthographic(orthographic),
		fov(fov)
	{}

	Camera::Camera(const Shaders::Uniform& uniform, const float fov, const float speed, const bool orthographic):
		camera(uniform),
		speed(speed),
		orthographic(orthographic),
		fov(fov)
	{
		setFOV(fov);
	}

	void Camera::setUniform(const Shaders::Uniform& uniform)
	{
		camera = uniform;
		setFOV(fov);
	}

	float Camera::getSpeed() const
	{
		return speed;
	}

	void Camera::setSpeed(const float speed)
	{
		this->speed = speed;
	}

	float Camera::getFOV() const
	{
		return fov;
	}

	void Camera::setFOV(const float fove)
	{
		if (orthographic)
		{
			camera.setMat4(glm::ortho(0.0f, (float)Engine::getWindow()->getWidth(), (float)Engine::getWindow()->getHeight(), 0.0f, 0.0f, 100.0f));
		}
		else
		{
			camera.setMat4(glm::perspective(glm::radians(fove),
				(float)Engine::getWindow()->getWidth() / (float)Engine::getWindow()->getHeight(), 0.1f, 100.0f));
		}

		fov = fove;
	}

	void Camera::setProjection(const bool orthographic)
	{
		this->orthographic = orthographic;

		setFOV(fov);
	}
}
