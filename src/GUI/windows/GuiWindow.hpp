#ifndef FEARENGINE_GUI_WINDOWS_GUIWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_GUIWINDOW_H__

#include <string>
#include <imgui.h>

namespace FearEngine::UI::windows
{
class GuiWindow
{
public:
	virtual void showWindow() = 0;

	bool isWindowOpen() const;

};
}
#endif
