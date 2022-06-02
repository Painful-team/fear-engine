#ifndef FEARENGINE_GUI_WINDOWS_GUIWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_GUIWINDOW_H__

#include <imgui.h>

namespace FearEngine
{
class Gui;
};

namespace FearEngine::UI::windows
{
class GuiWindow
{
public:
	virtual void init(FearEngine::Gui* main) = 0;

	virtual void showWindow() = 0;

	virtual bool isWindowOpen() const = 0;
};
}
#endif
