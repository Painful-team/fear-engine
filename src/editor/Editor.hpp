#ifndef FEARENGINE_GUI_GUI_H__
#define FEARENGINE_GUI_GUI_H__

#include <core/events.hpp>

#include <render/FrameBuffer.hpp>
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
struct EditorMainWindows
{
	void init();

	void showAllWindows();

	EditorUI::windows::SceneWindow sceneWindow;

	EditorUI::windows::HierarchyWindow hierarchyWindow;
	EditorUI::windows::ProjectWindow projectWindow;
	EditorUI::windows::InspectorWindow inspectorWindow;
	EditorUI::windows::HelpWindow helpWindow;

	EditorUI::windows::BottomPanel bottomPanel;
	EditorUI::windows::DockingArea dockingArea;
};

class Editor
{
public:
	Editor();
	int init();
	void resize(int width, int height);

	void begin();
	void end();

	bool onMouseMoved(Events::MouseMoved* e);
	bool onMousePressed(Events::MouseButtonPressed* e);
	bool onMouseReleased(Events::MouseButtonReleased* e);
	bool onMouseRequired(Events::MouseRequired* e);
	bool onScroll(Events::MouseScrolled* e);

	bool onKeyPressed(Events::KeyPressed* e);
	bool onKeyReleased(Events::KeyReleased* e);
	bool onKeyTyped(Events::KeyTyped* e);

	bool onResize(Events::WindowResize* e);

	~Editor();

	EditorMainWindows windows;
private:
	void applyInitialSettings();

	void setWindowStyles();
	void setFonts();
	void setMainColors();

	void showMainMenuBar();

	bool mouseReq;
};
}

#endif
