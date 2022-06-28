#include "ViewPort.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <core/Engine.hpp>
#include <components/CameraComponent.hpp>
#include <editor/Editor.hpp>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

FearEngine::EditorUI::windows::ViewPort::ViewPort()
 : enabled(false)
 , enabledGizmo(false)
 , size(0, 0)
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


		auto par = cam->getFrameBuffer().getParams();
		if (size.x != par.width || size.y != par.height)
		{
			cam->onResize(size.x, size.y);
		}
		size = contentRegion[1] - contentRegion[0];

		if (hovered && !ImGuizmo::IsOver())
		{
			auto evnt = Events::ActiveViewport(cam);
			Engine::getDispatcher()->notify(&evnt);
		}
	}
	//std::cout << &cam->getProjection() << std::endl;
	if (enabledGizmo)
	{
		auto& entity = Engine::getEditor()->windows.inspectorWindow.chosenEntity;

		if (entity.isValid())
		{
			auto& comp = Engine::getEditor()->windows.sceneWindow.editorCamera.getComponent<Component::Camera>();
			auto& trans = entity.getComponent<Component::Transform>();

			auto transform = trans.getTransformMatrix();

			// Todo add ability to change perspective on orthographic
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(contentRegion[0].x, contentRegion[0].y,
				 contentRegion[1].x - contentRegion[0].x,
				 contentRegion[1].y - contentRegion[0].y);

			ImGuizmo::Manipulate(glm::value_ptr(comp.getView()), glm::value_ptr(comp.getProjection()),
				 static_cast<ImGuizmo::OPERATION>(Engine::getEditor()->windows.sceneWindow.gizmoOperation), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform));
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 pos;
				glm::vec3 rotation;
				glm::vec3 scale;
				ImGuizmo::DecomposeMatrixToComponents(
					 glm::value_ptr(transform), glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale));

				trans.pos = pos;
				glm::vec3 deltaRotation = rotation - trans.rotation;
				trans.rotation += deltaRotation;
				trans.scale = scale;
			}
		}
	}

	ImGui::End();
}

bool FearEngine::EditorUI::windows::ViewPort::isWindowOpen() const { return false; }

bool FearEngine::EditorUI::windows::ViewPort::isPanelEnabled() const { return enabled; }

bool FearEngine::EditorUI::windows::ViewPort::isFocused() const { return hovered; }

void FearEngine::EditorUI::windows::ViewPort::togglePanel(const bool showPanel) {

}
