#include "BottomPanel.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <core/Engine.hpp>

namespace FearEngine::EditorUI::windows
{
BottomPanel::BottomPanel():
	panelEnabled(true),
	panelText("dev")
{}

void BottomPanel::init() {}

void BottomPanel::showWindow()
{
	const float panelHeight = 30.0f;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkPos.x, static_cast<float>(Engine::getWindow()->getHeight())
			- panelHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(Engine::getWindow()->getWidth()), panelHeight), ImGuiCond_Always);

	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoInputs;

	// color - #FFFFFFFF
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	ImGui::Begin("Bottom panel", &panelEnabled, windowFlags);
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(panelText.data()).x
				- 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);

		ImGui::Text(panelText.data());
	}
	ImGui::End();

	ImGui::PopStyleColor();
}


bool BottomPanel::isWindowOpen() const { return true; }

bool BottomPanel::isPanelEnabled() const { return panelEnabled; }


void BottomPanel::togglePanel(const bool showPanel)
{
	panelEnabled = showPanel;
}

const std::string& BottomPanel::getPanelText() const
{
	return panelText;
}

void BottomPanel::setPanelText(const std::string& newText)
{
	panelText = newText;
}
}
