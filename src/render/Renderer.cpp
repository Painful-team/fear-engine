#include <glad/glad.h>

#include "Renderer.hpp"

#include <fstream>
#include <iostream>

#include <GLFW/glfw3.h>

#include <core/Input.hpp>
#include <event/WindowEvent.hpp>

#include "Camera.hpp"
#include "Layer.hpp"
#include "shader/Shader.hpp"

#include <GUI/GUI.hpp>
#include "ObjLayer.hpp"

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);

	m_layers.emplace_back(new Render::ModelLayer);
	m_layers.emplace_back(new Gui);

	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		(*it)->init();
	}

	return 0;
}

void Renderer::preUpdate()
{
	assert(!m_layers.empty() && "Renderer not initialized");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::update()
{
	assert(!m_layers.empty() && "Renderer not initialized");

	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		(*it)->update();
	}
}

void Renderer::onResize(Events::WindowResize* event, const int x, const int y) { glViewport(x, y, event->getWidth(), event->getHeight()); }

Renderer::~Renderer()
{
	for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
	{
		delete (*it);
	}

	m_layers.clear();
}
}  // namespace FearEngine
