#ifndef FEARENGINE_GUI_WINDOWS_PROJECTWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_PROJECTWINDOW_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class ProjectWindow final : public GuiWindow
{
public:
	ProjectWindow();

	void showWindow() final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

	bool isFilterEnabled() const;
	void toggleFilterItem(const bool enableFilter);

private:
	bool windowOpen;
	bool filterItemEnabled;

	void showFilterMenuItem() const;
};
}
#endif
