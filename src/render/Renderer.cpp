#include <glad/glad.h>

#include "Renderer.hpp"

#include <fstream>
#include <iostream>

#include <GLFW/glfw3.h>

#include <core/Input.hpp>
#include <event/WindowEvent.hpp>

#include "Layer.hpp"
#include "shader/Shader.hpp"

#include <Editor/Editor.hpp>
#include "ObjLayer.hpp"
#include "DebugNormalLayer.hpp"

#include <core/Engine.hpp>
#include <event/CoreEvent.hpp>

namespace FearEngine
{
void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		 type, severity, message);
}

int Renderer::init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	initGraphicData();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
#endif

	auto evnt = Events::RenderInitialized();
	Engine::getDispatcher()->notify(&evnt);

	m_layers.emplace_back(new Render::DebugNormalsLayer);
	m_layers.emplace_back(new Render::ModelLayer);

	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		auto result = (*it)->init();
		assert(result == Render::errorCodes::OK);
	}

	return 0;
}

void Renderer::postUpdate()
{
}

void Renderer::preUpdate()
{
	assert(!m_layers.empty() && "Renderer not initialized");
}

void Renderer::update()
{
	assert(!m_layers.empty() && "Renderer not initialized");

	auto cameraView = Engine::getScene()->view<Component::Camera>();
	for (auto& entity : cameraView)
	{
		auto& camera = cameraView.get<Component::Camera>(entity);
		camera.getFrameBuffer().clear();
		for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
		{
			(*it)->preUpdate(camera);
			(*it)->update(camera);
			(*it)->postUpdate(camera);
		}
	}
}

void Renderer::onResize(Events::WindowResize* event, const int x, const int y)
{
	if (static_cast<int>(event->getWidth()) > 1 && static_cast<int>(event->getHeight()) > 1)
	{
		glViewport(x, y, event->getWidth(), event->getHeight());
	}

	for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		(*it)->resize(event->getWidth(), event->getHeight());
	}
}

Renderer::~Renderer()
{
	for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		delete (*it);
	}

	m_layers.clear();
}

int Renderer::initGraphicData()
{
	GLint maxBindings = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindings);

	graphicsData.emplace(GL_MAX_UNIFORM_BUFFER_BINDINGS, maxBindings);

	GLint maxBufferSize = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxBufferSize);
	graphicsData.emplace(GL_MAX_UNIFORM_BLOCK_SIZE, maxBufferSize);

	GLint maxTextureSlots = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
	graphicsData.emplace(GL_MAX_TEXTURE_IMAGE_UNITS, maxTextureSlots);
	return 0;
}
}  // namespace FearEngine
