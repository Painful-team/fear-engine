#ifndef FEARENGINE_GUI_GUI_H__
#define FEARENGINE_GUI_GUI_H__

#include <unordered_map>

#include <core/Engine.hpp>

#include "windows/GuiWindow.hpp"
#include "windows/SceneWindow.hpp"
#include "windows/HierarchyWindow.hpp"
#include "windows/ProjectWindow.hpp"
#include "windows/InspectorWindow.hpp"
#include "windows/HelpWindow.hpp"

namespace FearEngine
{
struct GuiMainWindows
{
	void showAllWindows(const UI::windows::WindowSettings* settings = nullptr)
	{
		sceneWindow.showWindow(settings);
		hierarchyWindow.showWindow(settings);
		projectWindow.showWindow(settings);
		inspectorWindow.showWindow(settings);
		helpWindow.showWindow(settings);
	}

	UI::windows::SceneWindow sceneWindow;
	UI::windows::HierarchyWindow hierarchyWindow;
	UI::windows::ProjectWindow projectWindow;
	UI::windows::InspectorWindow inspectorWindow;
	UI::windows::HelpWindow helpWindow;
};

class Gui
{
public:
	void init();
	void run();

	~Gui();

private:
	GuiMainWindows windows_;

	void onGui();

	void applyInitialSettings();

	void setWindowStyles();
	void setMainWindowFlags();
	void setFonts();
	void setMainColors();

	void showDockingArea();
	void showMainMenuBar();
	void showBottomPanel();

};
}

#endif
