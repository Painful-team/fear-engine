#ifndef FEARENGINE_GUI_WINDOWS_SCENEWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_SCENEWINDOW_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class SceneWindow final : public GuiWindow
{
public:
	SceneWindow();

	void showWindow() final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

	void setStatsItemSize(const ImVec2 newSize);

private:
	bool windowOpen;
	bool isSceneStarted;
	bool isScenePaused;

	ImVec2 statsItemSize;

	void showStatsDialog();

};
}
#endif
