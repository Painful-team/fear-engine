#ifndef FEARENGINE_GUI_WINDOWS_SCENEWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_SCENEWINDOW_H__

#include "GuiWindow.hpp"

#include "ViewPort.hpp"

#include <array>

#include <core/Entity.hpp>

namespace FearEngine
{
class Editor;
};

namespace FearEngine::EditorUI::windows
{
class SceneWindow final : public GuiWindow
{
public:
	static constexpr const uint16_t maxViewPorts = 16;

	SceneWindow();

	void init();

	void showWindow() final;

	bool isWindowOpen() const;
	bool isFocused() const;
	void toggleWindow(const bool openWindow);

	void setStatsItemSize(const ImVec2 newSize);

private:
	bool windowOpen;
	bool isSceneStarted;
	bool isScenePaused;
	bool focused;

	ImVec2 statsItemSize;

	void showStatsDialog();

	Entity editorCamera;

	std::array<ViewPort, 16> viewPorts;

	uint32_t cameralistSize;

	ImVec2 windowSize;

	friend class Editor;
};
}
#endif
