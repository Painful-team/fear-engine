#ifndef FEARENGINE_GUI_WINDOWS_VIEWPORT_H__
#define FEARENGINE_GUI_WINDOWS_VIEWPORT_H__

#include "GuiWindow.hpp"

#include <components/CameraComponent.hpp>

#include <string>

#include <memory>

namespace FearEngine
{
class Editor;
}

namespace FearEngine::EditorUI::windows
{
class ViewPort final: public GuiWindow
{
public:
	ViewPort();
	void init();

	void setCamera(Component::Camera* camera);
	Component::Camera* getCamera() const;

	void showWindow() final;
	bool isWindowOpen() const;

	bool isPanelEnabled() const;
	bool isFocused() const;
	void togglePanel(const bool showPanel);

	std::string name;
private:
	Component::Camera* cam;

	glm::vec2 size;
	glm::vec2 contentRegion[2];

	bool enabled;
	bool hovered;

	friend class Editor;
};
}  // namespace FearEngine::EditorUI::windows

#endif	//
