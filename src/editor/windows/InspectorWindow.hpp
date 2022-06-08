#ifndef FEARENGINE_GUI_WINDOWS_INSPECTORWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_INSPECTORWINDOW_H__

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "GuiWindow.hpp"

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

	glm::vec3 positionVec3;
	glm::vec3 rotateVec3;
	glm::vec3 scaleVec3;
	std::string modelPath;

	void showAddComponentBtn();
};
}  // namespace FearEngine::UI::windows
#endif