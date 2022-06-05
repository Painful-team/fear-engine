#ifndef FEARENGINE_GUI_WINDOWS_VIEWPORT_H__
#define FEARENGINE_GUI_WINDOWS_VIEWPORT_H__

#include "GuiWindow.hpp"

#include <render/Camera.hpp>

#include <string>

#include <memory>

namespace FearEngine::EditorUI::windows
{
class ViewPort final: public GuiWindow
{
public:
	ViewPort();
	void init();

	void setCamera(Render::Camera* camera);

	void showWindow() final;
	bool isWindowOpen() const;

	bool isPanelEnabled() const;
	bool isFocused() const;
	void togglePanel(const bool showPanel);

	std::string name; 
private:
	Render::Camera* cam;


	bool enabled;
	bool hovered;
};
}  // namespace FearEngine::EditorUI::windows

#endif	//
