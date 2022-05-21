#ifndef FEARENGINE_GUI_GUI_H__
#define FEARENGINE_GUI_GUI_H__

#include <core/events.hpp>

#include <render/Layer.hpp>

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
		if (sceneWindow.isWindowOpen())
		{
			sceneWindow.showWindow();
		}

		if (hierarchyWindow.isWindowOpen())
		{
			hierarchyWindow.showWindow();
		}

		if (projectWindow.isWindowOpen())
		{
			projectWindow.showWindow();
		}

		if (inspectorWindow.isWindowOpen())
		{
			inspectorWindow.showWindow();
		}

		if (helpWindow.isWindowOpen())
		{
			helpWindow.showWindow();
		}

		if (bottomPanel.isPanelEnabled())
		{
			bottomPanel.showWindow();
		}
	}

	UI::windows::SceneWindow sceneWindow;
	UI::windows::HierarchyWindow hierarchyWindow;
	UI::windows::ProjectWindow projectWindow;
	UI::windows::InspectorWindow inspectorWindow;
	UI::windows::HelpWindow helpWindow;

	UI::windows::BottomPanel bottomPanel;
	UI::windows::DockingArea dockingArea;
};

class Gui: public Render::Layer
{
public:
	void init();

	void update();

	bool onMouseMoved(Events::MouseMoved* e);
	bool onMousePressed(Events::MouseButtonPressed* e);
	bool onMouseReleased(Events::MouseButtonReleased* e);
	bool onScroll(Events::MouseScrolled* e);

	bool onKeyPressed(Events::KeyPressed* e);
	bool onKeyReleased(Events::KeyReleased* e);
	bool onKeyTyped(Events::KeyTyped* e);

	bool onResize(Events::WindowResize* e);

	~Gui();

private:
	GuiMainWindows windows;

	void applyInitialSettings();

	void setWindowStyles();
	void setFonts();
	void setMainColors();

	void showMainMenuBar();

};
}

#endif
