#ifndef FEARENGINE_RENDER_CAMERA_H__
#define FEARENGINE_RENDER_CAMERA_H__

#include <string>

#include "shader/Uniform.hpp"

namespace FearEngine::Render
{
class Camera
{
public:
	Camera(const Shaders::Uniform& uniform, const float fov = 90, const float speed = 0.1, const bool orthographic = false);
	Camera(const float fov = 90, const float speed = 2, const bool orthographic = false);

	void setUniform(const Shaders::Uniform& uniform);

	float getSpeed() const;
	void setSpeed(const float speed);

	float getFOV() const;
	void setFOV(const float fov);

	void setProjection(const bool orthographic);

	Shaders::Uniform camera;

	float fov;
	float speed;

private: 
	bool orthographic;
};
}  // namespace FearEngine::Render
#endif
