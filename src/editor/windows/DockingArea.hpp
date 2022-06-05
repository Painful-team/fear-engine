#ifndef FEARENGINE_GUI_WINDOWS_DOCKINGAREA_H__
#define FEARENGINE_GUI_WINDOWS_DOCKINGAREA_H__

#include "GuiWindow.hpp"

namespace FearEngine::EditorUI::windows
{
class DockingArea final : public GuiWindow
{
public:
	DockingArea();

	void init();

	void showWindow() final;

	bool isWindowOpen() const;
	bool isDockingEnabled() const;
	void toggleDocking(const bool enableDocking);

private:
	bool dockingEnabled;

};
}
#endif
