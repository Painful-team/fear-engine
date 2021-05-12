#include "HelpWindow.hpp"

#include <core/Engine.hpp>

namespace FearEngine::UI::windows
{
HelpWindow::HelpWindow() :
	isWindowOpen_(true)
{}

void HelpWindow::showWindow()
{
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

	const ImVec2 center = ImVec2(static_cast<float>(Engine::getWindow()->getWidth()) / 2.0f,
			static_cast<float>(Engine::getWindow()->getHeigth()) / 2.0f);
	const ImVec2 popupSize = ImVec2(500.0f, 300.0f);

	ImGui::OpenPopup("Help");

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(popupSize);

	if (ImGui::BeginPopupModal("Help", &isWindowOpen_, windowFlags))
	{
		const float buttonOffsetBottomY = 35.0f;

		ImGui::Text("There should be some helping guide ^_*");

		ImGui::SetCursorPosY(popupSize.y - buttonOffsetBottomY);
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			isWindowOpen_ = false;
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
	return isWindowOpen_;
}

void HelpWindow::toggleWindow(const bool openWindow)
{
	isWindowOpen_ = openWindow;
}
}
