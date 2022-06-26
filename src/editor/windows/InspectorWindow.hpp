#ifndef FEARENGINE_GUI_WINDOWS_INSPECTORWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_INSPECTORWINDOW_H__

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "GuiWindow.hpp"

#include <core/Entity.hpp>

namespace FearEngine::EditorUI::windows
{
class InspectorWindow final: public GuiWindow
{
public:
	InspectorWindow();

	void init();

	void showWindow() final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

	void updateObjectPicked(const bool objectPicked);
	void updateShowPickedObject(const bool showPicked);

	Entity chosenEntity;
private:
	enum InspectorComponents
	{
		Transform = 0,
		Color = 1,
		Mesh = 2,
		Interaction = 3,
		Shader = 4
	};

	bool windowOpen;
	bool isObjectPicked;

	bool showPickedObject;

	std::string pickedObjectName;
	float pickedObjectColor[4];

	std::vector<bool> componentsOpen;
	std::vector<std::string> componentsNames;

	std::string modelPath;

	void showAddComponentBtn();
};
}  // namespace FearEngine::UI::windows
#endif
