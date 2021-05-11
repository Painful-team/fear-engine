#ifndef FEARENGINE_CORE_GUI_H__
#define FEARENGINE_CORE_GUI_H__

#include <core/Engine.hpp>

namespace FearEngine
{
class Gui
{
public:
	void init();
	void run();

	~Gui();

private:
	void onGui();
	void applyInitialSettings();

	void setWindowStyles();
	void setMainWindowFlags();
	void setFonts();
	void setMainColors();

	void showDockingArea();
	void showMainMenuBar();
	void showBottomPanel();

	void showSceneWindow(bool*);
	void showHierarchyWindow(bool*);
	void showProjectWindow(bool*);
	void showInspectorWindow(bool*, bool);
	void showHelpWindow(bool*);

	void showStatsDialog();

};
}

#endif
