#ifndef FEARENGINE_GUI_GUI_H__
#define FEARENGINE_GUI_GUI_H__

#include <core/Engine.hpp>

#include "windows/GuiWindow.hpp"
#include "windows/SceneWindow.hpp"
#include "windows/HierarchyWindow.hpp"
#include "windows/ProjectWindow.hpp"
#include "windows/InspectorWindow.hpp"
#include "windows/HelpWindow.hpp"

#include "windows/BottomPanel.hpp"
#include "windows/DockingArea.hpp"

namespace FearEngine
{
struct GuiMainWindows
{
	void showAllWindows()
	{
		sceneWindow.showWindow();
		hierarchyWindow.showWindow();
		projectWindow.showWindow();
		inspectorWindow.showWindow();
		helpWindow.showWindow();

		bottomPanel.showWindow();
		dockingArea.showWindow();
	}

	UI::windows::SceneWindow sceneWindow;
	UI::windows::HierarchyWindow hierarchyWindow;
	UI::windows::ProjectWindow projectWindow;
	UI::windows::InspectorWindow inspectorWindow;
	UI::windows::HelpWindow helpWindow;

	UI::windows::BottomPanel bottomPanel;
	UI::windows::DockingArea dockingArea;
};

class Gui
{
public:
	void init();

	void onGui();

	~Gui();

private:
	GuiMainWindows windows;

	void applyInitialSettings();

	void setWindowStyles();
	void setMainWindowFlags();
	void setFonts();
	void setMainColors();

	void showMainMenuBar();

};
}

#endif
