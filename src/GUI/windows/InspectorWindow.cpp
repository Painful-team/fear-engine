#include "InspectorWindow.hpp"

#include <imgui_stdlib.h>

namespace FearEngine::UI::windows
{
InspectorWindow::InspectorWindow() :
	isWindowOpen_(true),
	isObjectPicked_(false),
	showPickedObject_(true),
	pickedObjectName_(""),
	componentsOpen_({ true, true, true, true, true }),
	componentsNames_({ "Transform", "Color", "Mesh", "Interaction", "Shader" }),
	transformValues_({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f })
{}

void InspectorWindow::showWindow(const WindowSettings* settings)
{
	const ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	if (!settings)
	{
		ImGui::SetNextWindowSizeConstraints(minWindowSize, minWindowSize);
	}
	else
	{
		ImGui::SetNextWindowSizeConstraints(minWindowSize, settings->maxWindowSize);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Inspector", &isWindowOpen_, ImGuiWindowFlags_NoCollapse);
	{
		if (isObjectPicked_)
		{
			{
				const float childOffsetTopY = 2.0f;
				const float nextNodeOffsetX = 18.0f;

				const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
						| ImGuiTreeNodeFlags_SpanAvailWidth;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childOffsetTopY);

				const ImVec2 childSize = ImVec2(ImGui::GetWindowWidth(), 40.0f);

				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

				if (ImGui::BeginChild("Object name area", childSize))
				{
					const float childObjectOffsetLeftX = 8.0f;
					const float childObjectOffsetTopY = 8.0f;

					const float inputTextCustomWidth = 130.0f;

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childObjectOffsetTopY);

					ImGui::Button("Tag");
					if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonLeft))
					{
						ImGui::Text("Select icon");

						ImGui::EndPopup();
					}
					ImGui::SameLine(65.0f);

					ImGui::PushItemWidth(1.0f);
					ImGui::Checkbox("##ShowPickedInspectorCheckbox", &showPickedObject_);
					ImGui::PopItemWidth();

					ImGui::SameLine(115.0f);

					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
					ImGui::PushItemWidth(inputTextCustomWidth);

					ImGui::InputText("##ObjectNameEditor", &pickedObjectName_);
					ImGui::PopStyleVar();
					ImGui::PopItemWidth();
				}
				ImGui::EndChild();

				ImGui::PopStyleColor();
			}

			if (ImGui::CollapsingHeader("Transform", componentsOpen_[InspectorComponents::Transform]))
			{
				const float childObjectOffsetLeftX = 8.0f;
				const float inputFloatCustomWidth = 70.0f;

				const float inputFloatOffsetCoeff = 60.0f;
				const float inputSameLineOffset = ImGui::GetWindowWidth() - childObjectOffsetLeftX
					- inputFloatCustomWidth * 3.0f - inputFloatOffsetCoeff;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Position");
				ImGui::SameLine(inputSameLineOffset);

				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##PositionInspectorX", &transformValues_[TransformValues::ObjectPositionX], 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Y##PositionInspectorY", &transformValues_[TransformValues::ObjectPositionY], 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Z##PositionInspectorZ", &transformValues_[TransformValues::ObjectPositionZ], 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Rotation");
				ImGui::SameLine(inputSameLineOffset);

				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##RotateInspectorX", &transformValues_[TransformValues::ObjectRotationX], 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Y##RotateInspectorY", &transformValues_[TransformValues::ObjectRotationY], 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Z##RotateInspectorZ", &transformValues_[TransformValues::ObjectRotationZ], 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Scale");
				ImGui::SameLine(inputSameLineOffset);

				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##ScaleInspectorX", &transformValues_[TransformValues::ObjectScaleX], 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Y##ScaleInspectorY", &transformValues_[TransformValues::ObjectScaleY], 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Z##ScaleInspectorZ", &transformValues_[TransformValues::ObjectScaleZ], 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}
			}

			if (ImGui::CollapsingHeader("Color", componentsOpen_[InspectorComponents::Color]))
			{
				const float childObjectOffsetLeftX = 70.0f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				float pickedObjectColor[] = { 0.4f, 0.7f, 0.0f, 0.5f };

				ImGui::ColorPicker4("color", pickedObjectColor);
			}

			if (ImGui::CollapsingHeader("Mesh", componentsOpen_[InspectorComponents::Mesh]))
			{

			}

			if (ImGui::CollapsingHeader("Interaction", componentsOpen_[InspectorComponents::Interaction]))
			{

			}

			if (ImGui::CollapsingHeader("Shader", componentsOpen_[InspectorComponents::Shader]))
			{

			}

			{
				const float btnOffsetRightX = 75.0f;
				const float btnOffsetTopY = 15.0f;

				const ImVec2 addComponentBtnSize = ImVec2(150.0f, 35.0f);

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() / 2.0f - btnOffsetRightX);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + btnOffsetTopY);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 250.0f);
				ImGui::Button("Add component", addComponentBtnSize);
				{
					if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonLeft))
					{
						for (int i = 0; i < componentsNames_.size(); ++i)
						{
							if (ImGui::Selectable(componentsNames_[i].c_str()))
							{
								componentsOpen_[i] = true;
							}
						}
						ImGui::EndPopup();
					}
				}
				ImGui::PopStyleVar();
			}
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

bool InspectorWindow::isWindowOpen() const
{
	return isWindowOpen_;
}

void InspectorWindow::toggleWindow(const bool openWindow)
{
	isWindowOpen_ = openWindow;
}
void InspectorWindow::updateObjectPicked(const bool objectPicked)
{
	isObjectPicked_ = objectPicked;
}

void InspectorWindow::updateShowPickedObject(const bool showPicked)
{
	showPickedObject_ = showPicked;
}
}
