#include "SceneWindow.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <core/Engine.hpp>

namespace FearEngine::UI::windows
{
SceneWindow::SceneWindow():
	isWindowOpen_(true),
	isSceneStarted_(false),
	isScenePaused_(false)
{}

void SceneWindow::showWindow()
{
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	const ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	const ImVec2 maxWindowSize = ImVec2(static_cast<float>(Engine::getWindow()->getWidth()),
			static_cast<float>(Engine::getWindow()->getHeigth()));

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Scene", &isWindowOpen_, windowFlags);
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

				if (isSceneStarted_ && !isScenePaused_)
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
					// Start scene
					isSceneStarted_ = true;
					isScenePaused_ = false;
				}

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f);

				if (!isSceneStarted_ || isScenePaused_)
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
					// Pause scene
					isScenePaused_ = true;
				}

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f + btnSize.x + btnOffset);

				if (!isSceneStarted_)
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
					// Stop scene
					isSceneStarted_ = false;
					isScenePaused_ = false;
				}
				ImGui::PopStyleColor(3);

				ImGui::PopItemFlag();
				ImGui::PopItemFlag();
				ImGui::PopItemFlag();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	ImGui::PopStyleVar();
}

bool SceneWindow::isWindowOpen() const
{
	return isWindowOpen_;
}

void SceneWindow::toggleWindow(const bool openWindow)
{
	isWindowOpen_ = openWindow;
}

void SceneWindow::showStatsDialog()
{
	ImGui::BeginChild("child", ImVec2(263.0f, 152.0f), false, ImGuiWindowFlags_NoBackground);
	{
		ImGui::Text("Polygons: %d");
		ImGui::Text("Objects: %d");
		ImGui::Text("Screen: %d");
	}
	ImGui::EndChild();
}
}
