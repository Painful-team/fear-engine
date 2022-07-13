#include "SceneWindow.hpp"
#include <Editor/Editor.hpp>

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

#include <components/CameraComponent.hpp>
#include <components/MaterialComponent.hpp>
#include <core/Engine.hpp>

#include <core/Input.hpp>

#include <glm/glm.hpp>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace FearEngine::EditorUI::windows
{
SceneWindow::SceneWindow()
 : windowOpen(true)
 , isSceneStarted(false)
 , isScenePaused(false)
 , windowSize(0, 0)
 , statsItemSize(260.0f, 150.0f)
 , cameralistSize(0)
 , gizmoOperation(ImGuizmo::OPERATION::TRANSLATE)
{}

void SceneWindow::init()
{
	Render::FrameBufferParams params;
	params.width = Engine::getWindow()->getWidth();
	params.height = Engine::getWindow()->getHeight();
	params.bufferTypes = Render::FrameBufferType::ColorAttachment0 | Render::FrameBufferType::ColorAttachment1
						 | Render::FrameBufferType::ColorAttachment2 | Render::FrameBufferType::ColorAttachment3
						 | Render::FrameBufferType::ColorAttachment4 | Render::FrameBufferType::ColorAttachment7
						 | Render::FrameBufferType::Depth | Render::FrameBufferType::Stencil;
	params.depthFormat = Render::DepthFormat::Depth24;
	params.stencilFormat = Render::StencilFormat::Stencil8;
	params.colorFormat[0] = Render::ColorFormat::RGB8;
	params.colorFormat[1] = Render::ColorFormat::RGBA8;
	params.colorFormat[2] = Render::ColorFormat::RGB8;
	params.colorFormat[3] = Render::ColorFormat::RGBA8;
	params.colorFormat[4] = Render::ColorFormat::RGB8;
	params.colorFormat[7] = Render::ColorFormat::R32;

	editorCamera = Engine::getScene()->createEntity("Scene Camera");
	auto& transform = editorCamera.getComponent<Component::Transform>();
	transform.pos = {1.88206, -0.279197, 2.37471};
	transform.rotation = {-6.39998, -128.8, 0};
	auto& cameraComponent = editorCamera.addComponent<Component::Camera>(&transform, params);
	editorCamera.addComponent<Component::EditorCamera>(&cameraComponent).initEvents();

	viewPorts[0].setCamera(&cameraComponent);
	viewPorts[0].name = "Scene Camera";
	viewPorts[0].enabledGizmo = true;
};

void SceneWindow::showWindow()
{
	const ImVec2 minWindowSize = ImVec2(400, 400);
	const ImVec2 maxWindowSize
		 = ImVec2(static_cast<float>(Engine::getWindow()->getWidth()), static_cast<float>(Engine::getWindow()->getHeight()));

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Scene", &windowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Scene stats"))
			{
				showStatsDialog();
				ImGui::EndMenu();
			}

			{
				const float btnOffset = 5.0f;
				const ImVec2 btnSize = ImVec2(24.0f, 24.0f);
				// color - #3A3A3AFF
				const ImVec4 btnOnColor = ImVec4(0.227f, 0.227f, 0.227f, 1.0f);
				// color - #292828FF
				const ImVec4 btnOffColor = ImVec4(0.151f, 0.157f, 0.157f, 1.0f);

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f - btnSize.x - btnOffset);

				if (isSceneStarted && !isScenePaused)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOffColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOnColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				if (ImGui::Button("|>", btnSize))
				{
					isSceneStarted = true;
					isScenePaused = false;
				}

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f);

				if (!isSceneStarted || isScenePaused)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOffColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOnColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				if (ImGui::Button("||", btnSize))
				{
					isScenePaused = true;
				}

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f + btnSize.x + btnOffset);

				if (!isSceneStarted)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOffColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOnColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				if (ImGui::Button("|=|", btnSize))
				{
					isSceneStarted = false;
					isScenePaused = false;
				}
				ImGui::PopStyleColor(3);
				ImGui::PopItemFlag();
				ImGui::PopItemFlag();
				ImGui::PopItemFlag();
			}

			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Debug").x - 10);
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::BeginChild("DebugChild", ImVec2(260.0f, 150.0f), false, ImGuiWindowFlags_NoBackground);
				{
					static bool debugProperty = false;
					bool normals = debugProperty;
					ImGui::Checkbox("Normals", &normals);
					if (debugProperty != normals)
					{
						debugProperty = normals;
						if (debugProperty)
						{
							Engine::getRender()->enabledDebugProperties |= Render::debugProperties::Normals;
						}
						else
						{
							Engine::getRender()->enabledDebugProperties -= Render::debugProperties::Normals;
						}
					}

					static bool sceneOnlyObjects = false;
					bool sceneObjects = sceneOnlyObjects;
					ImGui::Checkbox("Scene onlyObjects", &sceneObjects);
					if (sceneOnlyObjects != sceneObjects)
					{
						sceneOnlyObjects = sceneObjects;
						if (sceneOnlyObjects)
						{
							Engine::getRender()->enabledDebugProperties |= Render::debugProperties::SceneObjects;
						}
						else
						{
							Engine::getRender()->enabledDebugProperties -= Render::debugProperties::SceneObjects;
						}
					}
				}

				ImGui::EndChild();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		auto size = ImGui::GetContentRegionAvail();
		ImGuiID sceneDockSpaceId = ImGui::GetID("sceneViewPortDock");
		ImGuiWindowClass viewPortClass;
		viewPortClass.ClassId = sceneDockSpaceId;
		viewPortClass.DockNodeFlagsOverrideSet = 0;
		viewPortClass.DockingAllowUnclassed = false;
		ImGui::DockSpace(sceneDockSpaceId, size, 0, &viewPortClass);
		auto view = Engine::getScene()->view<Component::Camera, Component::Tag>();

		for (auto& entity : view)
		{
			auto& [cam, tag] = view.get<Component::Camera, Component::Tag>(entity);
			for (uint32_t i = 0; i < maxViewPorts; ++i)
			{
				if (&viewPorts[i].getCamera() == &cam)
				{
					break;
				}

				if (!viewPorts[i].isPanelEnabled())
				{
					viewPorts[i].setCamera(&cam);
					viewPorts[i].name = "Viewport " + tag.tag;
					break;
				}
			}
		}

		focused = false;
		for (uint16_t i = 0; i < maxViewPorts; ++i)
		{
			if (viewPorts[i].isPanelEnabled())
			{
				ImGui::SetNextWindowClass(&viewPortClass);

				viewPorts[i].showWindow();

				focused |= viewPorts[i].isFocused();
				focused |= ImGui::IsItemHovered();
			}
		}

		ImGui::End();
	}
	ImGui::PopStyleVar();
}

bool SceneWindow::isWindowOpen() const { return windowOpen; }

bool SceneWindow::isFocused() const { return focused; }

void SceneWindow::toggleWindow(const bool openWindow) { windowOpen = openWindow; }

void SceneWindow::setStatsItemSize(const ImVec2 newSize) { statsItemSize = newSize; }

void SceneWindow::showStatsDialog()
{
	ImGui::BeginChild("child", statsItemSize, false, ImGuiWindowFlags_NoBackground);
	{
		ImGui::Text("Polygons: %d", Engine::getRender()->stats.polygons);
		ImGui::Text("DrawCalls: %d", Engine::getRender()->stats.drawCalls);
		ImGui::Text("Objects: %d", Engine::getScene()->view<Component::Renderable>().size());
	}
	ImGui::EndChild();
}
}  // namespace FearEngine::EditorUI::windows
