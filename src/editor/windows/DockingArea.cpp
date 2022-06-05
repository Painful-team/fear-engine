#include "DockingArea.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace FearEngine::EditorUI::windows
{
DockingArea::DockingArea():
	dockingEnabled(true)
{}

void DockingArea::init() {}

void DockingArea::showWindow()
{
	const float bottomPanelHeight = 30.0f;

	
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
			| ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	const ImVec2 dockingAreaSize = ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - bottomPanelHeight);
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(dockingAreaSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Docker window", &dockingEnabled, windowFlags);

	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}
	
	ImGui::End();
}

bool DockingArea::isDockingEnabled() const
{
	return dockingEnabled;
}

bool DockingArea::isWindowOpen() const
{ return true; }


void DockingArea::toggleDocking(const bool enableDocking)
{
	dockingEnabled = enableDocking;
}
}
