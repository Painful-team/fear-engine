#ifndef FEARENGINE_GUI_WINDOWS_HELPWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_HELPWINDOW_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class HelpWindow final : public GuiWindow
{
public:
	HelpWindow();

	void init(Gui* layer);
		
	void showWindow() final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

private:
	bool windowOpen;

};
}
#endif
