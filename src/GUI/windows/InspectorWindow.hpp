#ifndef FEARENGINE_GUI_WINDOWS_INSPECTORWINDOW_H__
#define FEARENGINE_GUI_WINDOWS_INSPECTORWINDOW_H__

#include <vector>
#include <string>

#include "GuiWindow.hpp"

namespace FearEngine::UI::windows
{
class InspectorWindow final : public GuiWindow
{
public:
	InspectorWindow();

	void showWindow(const WindowSettings* settings = nullptr) final;

	bool isWindowOpen() const;
	void toggleWindow(const bool openWindow);

	void updateObjectPicked(const bool objectPicked);
	void updateShowPickedObject(const bool showPicked);

private:
	enum InspectorComponents
	{
		Transform						= 0,
		Color								= 1,
		Mesh								= 2,
		Interaction					= 3,
		Shader							= 4
	};

	enum TransformValues
	{
		ObjectPositionX			= 0,
		ObjectPositionY			= 1,
		ObjectPositionZ			= 2,

		ObjectRotationX			= 3,
		ObjectRotationY			= 4,
		ObjectRotationZ			= 5,

		ObjectScaleX				= 6,
		ObjectScaleY				= 7,
		ObjectScaleZ				= 8
	};

	bool isWindowOpen_;
	bool isObjectPicked_;
	
	bool showPickedObject_;

	std::string pickedObjectName_;

	std::vector<bool> componentsOpen_;
	std::vector<std::string> componentsNames_;
	std::vector<float> transformValues_;

};
}
#endif
