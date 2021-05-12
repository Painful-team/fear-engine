#ifndef FEARENGINE_UI_WINDOWS_GUIWINDOW_H__
#define FEARENGINE_UI_WINDOWS_GUIWINDOW_H__

#include <string>
#include <imgui.h>

namespace FearEngine::UI::windows
{
struct WindowSettings
{
	ImVec2 maxWindowSize;
};

class GuiWindow
{
public:
	virtual void showWindow(const WindowSettings* settings) = 0;

};
}
#endif
