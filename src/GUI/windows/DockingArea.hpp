#ifndef FEARENGINE_GUI_WINDOWS_DOCKINGAREA_H__
#define FEARENGINE_GUI_WINDOWS_DOCKINGAREA_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class DockingArea final : public GuiWindow
{
public:
	DockingArea();

	void showWindow() final;

	bool isDockingEnabled() const;
	void toggleDocking(const bool enableDocking);

private:
	bool dockingEnabled;

};
}
#endif
