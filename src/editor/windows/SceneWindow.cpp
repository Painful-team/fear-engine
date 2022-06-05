#include "SceneWindow.hpp"
#include <Editor/Editor.hpp>

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

#include <core/Engine.hpp>

namespace FearEngine::EditorUI::windows
{
SceneWindow::SceneWindow()
 : windowOpen(true)
 , isSceneStarted(false)
 , isScenePaused(false)
 , windowSize(0, 0)
 , statsItemSize(260.0f, 150.0f)
 , cameralistSize(0)
 , cameraPos(0)
{}

void SceneWindow::init() {};

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
			if (ImGui::BeginMenu("Stats"))
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
			ImGui::EndMenuBar();
		}

		auto size = ImGui::GetContentRegionAvail();
		ImGuiID sceneDockSpaceId = ImGui::GetID("sceneViewPortDock");
		ImGuiWindowClass viewPortClass;
		viewPortClass.ClassId = sceneDockSpaceId;
		viewPortClass.DockNodeFlagsOverrideSet = 0;
		viewPortClass.DockingAllowUnclassed = false;
		ImGui::DockSpace(sceneDockSpaceId, size, 0, &viewPortClass);
		
		if (Engine::getRender()->cameras.size() != cameralistSize && Engine::getRender()->cameras.size() != 0)
		{
			int tmpCamPos = -1;
			for (auto& cam : Engine::getRender()->cameras)
			{
				++tmpCamPos;
				if (cameraPos > tmpCamPos)
				{
					continue;
				}

				for (uint32_t i = 0; i < maxViewPorts; ++i)
				{
					if (!viewPorts[i].isPanelEnabled())
					{
						viewPorts[i].setCamera(&cam);
						viewPorts[i].name = "Viewport " + std::to_string(i);
						break;
					}
				}
			}

			cameralistSize = Engine::getRender()->cameras.size();
			cameraPos = cameralistSize;
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
		ImGui::Text("Polygons: %d", 1);
		ImGui::Text("Objects: %d", 1);
		ImGui::Text("Screen: %d", 1);
	}
	ImGui::EndChild();
}
}  // namespace FearEngine::UI::windows
