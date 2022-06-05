#include "HelpWindow.hpp"

#include <core/Engine.hpp>

namespace FearEngine::EditorUI::windows
{
HelpWindow::HelpWindow():
	windowOpen(false)
{}

void HelpWindow::init() {}

void HelpWindow::showWindow()
{
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

	const ImVec2 center = ImVec2(static_cast<float>(Engine::getWindow()->getWidth()) / 2.0f,
			static_cast<float>(Engine::getWindow()->getHeight()) / 2.0f);
	const ImVec2 popupSize = ImVec2(500.0f, 300.0f);

	ImGui::OpenPopup("Help");

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(popupSize);

	if (ImGui::BeginPopupModal("Help", &windowOpen, windowFlags))
	{
		const float buttonOffsetBottomY = 35.0f;

		ImGui::Text("There should be some helping guide ^_*");

		ImGui::SetCursorPosY(popupSize.y - buttonOffsetBottomY);
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			windowOpen = false;
		}

		ImGui::SameLine(popupSize.x);

		if (ImGui::Button("Next"))
		{
		}

		ImGui::EndPopup();
	}
}

bool HelpWindow::isWindowOpen() const
{
	return windowOpen;
}

void HelpWindow::toggleWindow(const bool openWindow)
{
	windowOpen = openWindow;
}
}
