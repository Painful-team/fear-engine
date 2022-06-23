#include "CameraComponent.hpp"

#include <core/Engine.hpp>

#include <core/events.hpp>

namespace FearEngine::Component
{
Camera::Camera(Transform* camTransform,
	 Render::FrameBufferParams& params,
	 float fieldOfView,
	 float near,
	 float far, bool isOrthograpic)
 : transform(camTransform)
 , fov(fieldOfView)
 , nearPlane(near)
 , farPlane(far)
 , orthographic(isOrthograpic)
{
	frameBuffer.init(params);
	setFOV(fov);
	updateCameraPos();
}

Camera::Camera(Camera&& other) noexcept { *this = std::move(other); }

void Camera::beginView()
{
	frameBuffer.enable();
	updateUniformData();
}

void Camera::end() { frameBuffer.disable(); }

void Camera::setUniforms(const Render::Shaders::Uniform& uniform, const Render::Shaders::Uniform& view)
{
	cameraUn = uniform;
	viewUn = view;
	setFOV(fov);
	updateCameraPos();
}

float Camera::getFOV() const { return fov; }

void Camera::setFOV(const float fove)
{
	auto params = frameBuffer.getParams();
	if (orthographic)
	{
		auto aspectRation = (float)params.width / (float)params.height;
		projection = glm::ortho(-aspectRation, aspectRation, -0.1f, 0.1f, -1.0f, 1.0f);
	}
	else
	{
		projection = glm::perspective(glm::radians(fove), (float)params.width / (float)params.height, nearPlane, farPlane);
	}

	fov = fove;
}

inline float Camera::getNear() const { return nearPlane; }

inline float Camera::getFar() const { return farPlane; }

inline void Camera::setNear(float camNearPlane)
{
	nearPlane = camNearPlane;
	setFOV(fov);
}

inline void Camera::setFar(float camFarPlane)
{
	farPlane = camFarPlane;
	setFOV(fov);
}

void Camera::updateCameraPos()
{
	glm::vec3 frontAngle;
	frontAngle.x = cos(glm::radians(transform->rotation.y)) * cos(glm::radians(transform->rotation.x));
	frontAngle.y = sin(glm::radians(transform->rotation.x));
	frontAngle.z = sin(glm::radians(transform->rotation.y)) * cos(glm::radians(transform->rotation.x));
	front = glm::normalize(frontAngle);
}

Render::FrameBuffer& Camera::getFrameBuffer() { return frameBuffer; };
//void Camera::setFrameBuffer(Render::FrameBuffer& buffer) {}	 // frameBuffer = buffer; }

bool Camera::isOrthograpic() { return orthographic; }

void Camera::setProjection(const bool isOrthographic)
{
	orthographic = isOrthographic;

	setFOV(fov);
}

const glm::mat4& Camera::getProjection() const { return projection; }

glm::mat4 Camera::getView() const { return glm::lookAt(transform->pos, transform->pos + front, cameraUp); }

bool NoclipCameraController::onMove(FearEngine::Events::MouseMoved* evnt)
{
	if (camera == nullptr || !flyActive || !isInputEnabled)
	{
		return true;
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

	camera->transform->rotation.y += xOffset;
	camera->transform->rotation.x += yOffset;

	if (camera->transform->rotation.x > 89.0f)
	{
		camera->transform->rotation.x = 89.0f;
	}
	else if (camera->transform->rotation.x < -89.0f)
	{
		camera->transform->rotation.x = -89.0f;
	}

	camera->updateCameraPos();
	return true;
}

bool NoclipCameraController::onMousePressed(Events::MouseButtonPressed* evnt)
{
	if (!isInputEnabled || camera == nullptr || evnt->getButton() != Events::Mouse::BUTTON_RIGHT)
	{
		return true;
	}

	Engine::getWindow()->blockCursor();
	flyActive = true;

	auto reqEvnt = Events::MouseRequired(true);
	Engine::getDispatcher()->notify(&reqEvnt);

	return true;
}

bool NoclipCameraController::onMouseReleased(Events::MouseButtonReleased* evnt)
{
	if (!isInputEnabled || camera == nullptr || evnt->getButton() != Events::Mouse::BUTTON_RIGHT)
	{
		return true;
	}

	Engine::getWindow()->unblockCursor();

	flyActive = false;
	flyInitialized = false;

	auto reqEvnt = Events::MouseRequired(false);
	Engine::getDispatcher()->notify(&reqEvnt);

	return true;
}

bool NoclipCameraController::onMouseScrolled(Events::MouseScrolled* evnt) { return true; }

bool NoclipCameraController::onKeyPressed(Events::KeyPressed* evnt) { return true; }

bool NoclipCameraController::onKeyReleased(Events::KeyReleased* evnt) { return true; }

// Todo add timebased movement to be able to stop relying on frames
bool NoclipCameraController::onKeyTyped(Events::KeyTyped* evnt)
{
	if (camera == nullptr || !isInputEnabled)
	{
		return true;
	}

	bool moved = false;
	// Todo add ability to remap keys
	if (evnt->keyCode() == Events::Key::W)
	{
		camera->transform->pos += speed * camera->front;
	}

	if (evnt->keyCode() == Events::Key::S)
	{
		camera->transform->pos -= speed * camera->front;
	}

	if (evnt->keyCode() == Events::Key::A)
	{
		camera->transform->pos -= glm::normalize(glm::cross(camera->front, camera->cameraUp)) * speed;
	}

	if (evnt->keyCode() == Events::Key::D)
	{
		camera->transform->pos += glm::normalize(glm::cross(camera->front, camera->cameraUp)) * speed;
	}

	camera->updateCameraPos();
	return true;
}

bool NoclipCameraController::onActiveCamera(Events::ActiveViewport* ent)
{
	isInputEnabled = ent->getActive()->transform == camera->transform;

	return true;
}

void NoclipCameraController::onResize(int width, int height)
{
	if (camera != nullptr && width > 1 && height > 1)
	{
		auto params = camera->frameBuffer.getParams();
		params.width = width;
		params.height = height;

		camera->frameBuffer.setParams(params);
		camera->setFOV(camera->fov);
	}
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	cameraUn = std::move(other.cameraUn);
	farPlane = other.farPlane;
	fov = other.fov;
	frameBuffer = std::move(other.frameBuffer);
	front = other.front;
	nearPlane = other.nearPlane;
	orthographic = other.orthographic;
	projection = other.projection;
	transform = other.transform;
	viewUn = std::move(other.viewUn);

	//other.transform = nullptr;

	return *this;
}

void Camera::updateUniformData()
{
	viewUn.setMat4(glm::lookAt(transform->pos, transform->pos + front, cameraUp));
	cameraUn.setMat4(projection);
}

NoclipCameraController::NoclipCameraController(Camera* cam, float camSpeed, const glm::vec2& camSensivity)
 : camera(cam)
 , speed(camSpeed)
 , sensivity(camSensivity)
{}

NoclipCameraController::NoclipCameraController(NoclipCameraController&& other) noexcept
{ *this = std::move(other); }

NoclipCameraController& NoclipCameraController::operator=(NoclipCameraController && other) noexcept
{
	camera = other.camera;
	flyInitialized = other.flyInitialized;
	flyActive = other.flyActive;
	speed = other.speed;
	sensivity = other.sensivity;
	mousePos = other.mousePos;
	isInputEnabled = other.isInputEnabled;
	initEvents();
	other.camera = nullptr;
	other.flyInitialized = false;
	other.flyActive = false;

	other.detachEvents();

	return *this;
}

void NoclipCameraController::initEvents()
{
	eventInitialized = true;
	attachedEventHandles[0] = Engine::getDispatcher()->get<Events::MouseMoved>()->attach<&NoclipCameraController::onMove>(this);
	attachedEventHandles[1] = Engine::getDispatcher()->get<Events::MouseButtonPressed>()->attach<&NoclipCameraController::onMousePressed>(this);
	attachedEventHandles[2] = Engine::getDispatcher()->get<Events::MouseButtonReleased>()->attach<&NoclipCameraController::onMouseReleased>(this);
	attachedEventHandles[3] = Engine::getDispatcher()->get<Events::MouseScrolled>()->attach<&NoclipCameraController::onMouseScrolled>(this);
	attachedEventHandles[4] = Engine::getDispatcher()->get<Events::KeyPressed>()->attach<&NoclipCameraController::onKeyPressed>(this);
	attachedEventHandles[5] = Engine::getDispatcher()->get<Events::KeyReleased>()->attach<&NoclipCameraController::onKeyReleased>(this);
	attachedEventHandles[6] = Engine::getDispatcher()->get<Events::KeyTyped>()->attach<&NoclipCameraController::onKeyTyped>(this);
	attachedEventHandles[7] = Engine::getDispatcher()->get<Events::ActiveViewport>()->attach<&NoclipCameraController::onActiveCamera>(this);
}

void NoclipCameraController::detachEvents()
{
	if (!eventInitialized)
	{
		return;
	}

	Engine::getDispatcher()->get<Events::MouseMoved>()->detach(attachedEventHandles[0]);
	Engine::getDispatcher()->get<Events::MouseButtonPressed>()->detach(attachedEventHandles[1]);
	Engine::getDispatcher()->get<Events::MouseButtonReleased>()->detach(attachedEventHandles[2]);
	Engine::getDispatcher()->get<Events::MouseScrolled>()->detach(attachedEventHandles[3]);
	Engine::getDispatcher()->get<Events::KeyPressed>()->detach(attachedEventHandles[4]);
	Engine::getDispatcher()->get<Events::KeyReleased>()->detach(attachedEventHandles[5]);
	Engine::getDispatcher()->get<Events::KeyTyped>()->detach(attachedEventHandles[6]);
	Engine::getDispatcher()->get<Events::ActiveViewport>()->detach(attachedEventHandles[7]);
}

NoclipCameraController::~NoclipCameraController()
{ detachEvents(); }

}  // namespace FearEngine::Component
