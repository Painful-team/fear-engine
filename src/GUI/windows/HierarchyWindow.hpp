#ifndef FEARENGINE_UI_WINDOWS_HIERARCHY_H__
#define FEARENGINE_UI_WINDOWS_HIERARCHY_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class HierarchyWindow final : public GuiWindow
{
public:
	HierarchyWindow();

	void showWindow(const WindowSettings* settings = nullptr) final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

private:
	bool isWindowOpen_;

};
}
#endif
