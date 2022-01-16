#ifndef FEARENGINE_GUI_WINDOWS_HIERARCHYWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_HIERARCHYWINDOW_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class HierarchyWindow final : public GuiWindow
{
public:
	HierarchyWindow();

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
