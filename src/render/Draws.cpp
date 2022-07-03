#include "glad/glad.h"
#include "Draws.hpp"

#include <core/Engine.hpp>

#include "shader/Shader.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"

#include <array>

namespace FearEngine::Render::Draws
{
namespace
{
uint32_t lineCount = 0;

constexpr const uint32_t maxOutlineVertices = 1000;
std::array<OutlineVertice, maxOutlineVertices> lines;

Buffer vertex({{BufferType::Float, 3}, {BufferType::Float, 3}});
VertexArray arr;

Shaders::Shader lineShader;

Shaders::Uniform projection;
Shaders::Uniform view;
}  // namespace

errorCode initOutlines()
{
	auto result = lineShader.readShader("resources/shaders/lines/Line.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = lineShader.readShader("resources/shaders/lines/Line.frag", Shaders::ShaderType::Fragment);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = lineShader.compile();
	if (result != errorCodes::OK)
	{
		return result;
	}

	lineShader.use();

	vertex.genBuffer();
	arr.genArray();

	arr.bind();

	vertex.bindData(sizeof(OutlineVertice) * maxOutlineVertices);
	arr.addVertexBuffer(vertex);

	vertex.unbind();
	arr.unBind();

	lineShader.use();

	projection = lineShader.findUniform("projection");
	view = lineShader.findUniform("view");

	return errorCodes::OK;
}

void clearOutlines() { lineCount = 0; }

void drawOutlines(OutlineVertice* vertices, uint32_t size)
{
	assert(vertices != nullptr);

	assert(lineCount < maxOutlineVertices);

	uint32_t i = 0;
	for (; i < size - 1; ++i)
	{
		memcpy(lines.data() + lineCount + (2 * i), vertices + i, 2 * sizeof(OutlineVertice));
	}

	if (size > 1)
	{
		lines[lineCount + i * 2] = vertices[size - 1];
		lines[lineCount + i * 2 + 1] = vertices[0];
	}
	else if (size == 1)
	{
		lines[lineCount + i * 2] = vertices[0];
		lines[lineCount + i * 2 + 1] = vertices[0];
	}

	lineCount += size * 2;
}

void submitOutlines(Component::Camera& cam)
{
	if (lineCount == 0)
	{
		return;
	}

	lineShader.use();
	view.setMat4(cam.getView());
	projection.setMat4(cam.getProjection());

	uint32_t skipAttachment = 1;
	cam.beginView(&skipAttachment, 1);

	arr.bind();
	vertex.setData(reinterpret_cast<float*>(lines.data()), lineCount * sizeof(OutlineVertice));
	glDrawArrays(GL_LINES, 0, lineCount);

	cam.end();
}

void draw(VertexArray& arr, uint32_t verticesCount)
{
	arr.bind();
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

	++Engine::getRender()->stats.drawCalls;
	Engine::getRender()->stats.polygons += verticesCount / 3;
}

void drawIndexed(VertexArray& arr, uint32_t verticesCount, uint32_t objCount)
{
	arr.bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, verticesCount, objCount);

	++Engine::getRender()->stats.drawCalls;
	Engine::getRender()->stats.polygons += verticesCount * objCount / 3;
}
}  // namespace FearEngine::Render::Draws
