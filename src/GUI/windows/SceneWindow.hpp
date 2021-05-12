#ifndef FEARENGINE_GUI_WINDOWS_SCENE_H__
#define FEARENGINE_GUI_WINDOWS_SCENE_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class SceneWindow final : public GuiWindow
{
public:
	SceneWindow();

	void showWindow(const WindowSettings* settings = nullptr) final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

private:
	bool isWindowOpen_;
	bool isSceneStarted_;
	bool isScenePaused_;

	void showStatsDialog();

};
}
#endif
