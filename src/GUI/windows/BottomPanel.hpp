#ifndef FEARENGINE_GUI_WINDOWS_BOTTOMPANEL_H__
#define FEARENGINE_GUI_WINDOWS_BOTTOMPANEL_H__

#include <string>

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class BottomPanel final : public GuiWindow
{
public:
	BottomPanel();

	void showWindow() final;

	bool isPanelEnabled() const;
	void togglePanel(const bool showPanel);

	const std::string& getPanelText() const;
	void setPanelText(const std::string& newText);

private:
	bool panelEnabled;
	std::string panelText;

};
}
#endif
