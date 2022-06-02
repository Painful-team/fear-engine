#include "SceneWindow.hpp"
#include <GUI/GUI.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <core/Engine.hpp>

#include <iostream>

namespace FearEngine::UI::windows
{
SceneWindow::SceneWindow()
 : mainLayer(nullptr)
 , windowOpen(true)
 , isSceneStarted(false)
 , isScenePaused(false)
 , windowSize(0, 0)
 , statsItemSize(260.0f, 150.0f)
{}

void SceneWindow::init(Gui* layer) 
{ 
	mainLayer = layer; 
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
		ImGui::Image((void*)mainLayer->getFrameBuffer().getColorAttachment(), size, {0, 1}, {1, 0});

		if (size.x != windowSize.x || size.y != windowSize.y)
		{
			windowSize = size;

			auto evnt = Events::WindowResize(size.x, size.y);
			Engine::getDispatcher()->notify(&evnt);
		}

		ImGui::End();
	}

	ImGui::PopStyleVar();
}

bool SceneWindow::isWindowOpen() const { return windowOpen; }

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
