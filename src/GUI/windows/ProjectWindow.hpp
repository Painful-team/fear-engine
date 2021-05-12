#ifndef FEARENGINE_UI_WINDOWS_PROJECT_H__
#define FEARENGINE_UI_WINDOWS_PROJECT_H__

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class ProjectWindow final : public GuiWindow
{
public:
	ProjectWindow();

	void showWindow(const WindowSettings* settings = nullptr) final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

private:
	bool isWindowOpen_;

};
}
#endif
