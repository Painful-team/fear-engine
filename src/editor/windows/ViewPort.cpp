#include "ViewPort.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <core/Engine.hpp>

FearEngine::EditorUI::windows::ViewPort::ViewPort()
 : enabled(false)
{}

void FearEngine::EditorUI::windows::ViewPort::init() {}

void FearEngine::EditorUI::windows::ViewPort::setCamera(FearEngine::Render::Camera* camera) { 
	cam = camera; 
	enabled = true;
}

void FearEngine::EditorUI::windows::ViewPort::showWindow()
{
	ImGui::Begin(name.c_str());
	hovered = ImGui::IsItemHovered();
	if (enabled)
	{
		auto size = ImGui::GetContentRegionAvail();
		if (cam->getFrameBuffer().isInitialized())
		{
			ImGui::Image((void*)cam->getFrameBuffer().getColorAttachment(), size, {0, 1}, {1, 0});
		}
		hovered |= ImGui::IsItemHovered();
	}
	ImGui::End();
}

bool FearEngine::EditorUI::windows::ViewPort::isWindowOpen() const { return false; }

bool FearEngine::EditorUI::windows::ViewPort::isPanelEnabled() const { return enabled; }

bool FearEngine::EditorUI::windows::ViewPort::isFocused() const { return hovered; }

void FearEngine::EditorUI::windows::ViewPort::togglePanel(const bool showPanel) {

}
