#include "ViewPort.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <core/Engine.hpp>

FearEngine::EditorUI::windows::ViewPort::ViewPort()
 : enabled(false)
{}

void FearEngine::EditorUI::windows::ViewPort::init() {}

void FearEngine::EditorUI::windows::ViewPort::setCamera(Component::Camera* camera)
{
	cam = camera;
	enabled = true;
}

FearEngine::Component::Camera* FearEngine::EditorUI::windows::ViewPort::getCamera() const { return cam; }

void FearEngine::EditorUI::windows::ViewPort::showWindow()
{
	ImGui::Begin(name.c_str());

	ImGui::SetWindowSize({400, 400}, ImGuiCond_::ImGuiCond_Once);

	hovered = ImGui::IsItemHovered();
	if (enabled)
	{
		auto imPos = ImGui::GetWindowPos();
		auto imContentMin = ImGui::GetWindowContentRegionMin();
		auto imContentMax = ImGui::GetWindowContentRegionMax();
		contentRegion[0] = {imContentMin.x + imPos.x, imContentMin.y + imPos.y};
		contentRegion[1] = {imContentMax.x + imPos.x, imContentMax.y + imPos.y};

		auto imSize = ImGui::GetContentRegionAvail();
		if (cam->getFrameBuffer().isInitialized())
		{
			ImGui::Image((void*)cam->getFrameBuffer().getColorAttachment(), imSize, {0, 1}, {1, 0});
		}
		hovered |= ImGui::IsItemHovered();

		if (size != contentRegion[1] - contentRegion[0])
		{
			cam->onResize(size.x, size.y);
		}
		size = contentRegion[1] - contentRegion[0];

		if (hovered)
		{
			auto evnt = Events::ActiveViewport(cam);
			Engine::getDispatcher()->notify(&evnt);
		}
	}
	ImGui::End();
}

bool FearEngine::EditorUI::windows::ViewPort::isWindowOpen() const { return false; }

bool FearEngine::EditorUI::windows::ViewPort::isPanelEnabled() const { return enabled; }

bool FearEngine::EditorUI::windows::ViewPort::isFocused() const { return hovered; }

void FearEngine::EditorUI::windows::ViewPort::togglePanel(const bool showPanel) {

}
