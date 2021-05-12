#include "InspectorWindow.hpp"

#include <core/Engine.hpp>

namespace FearEngine::UI::windows
{
InspectorWindow::InspectorWindow():
	windowOpen(true),
	isObjectPicked(false),
	showPickedObject(true),
	pickedObjectName(""),
	componentsOpen({ true, true, true, true, true }),
	componentsNames({ "Transform", "Color", "Mesh", "Interaction", "Shader" }),
	positionVec3({0.0f, 0.0f, 0.0f}),
	rotateVec3({ 0.0f, 0.0f, 0.0f }),
	scaleVec3({ 0.0f, 0.0f, 0.0f })
{
	for (int i = 0; i < IM_ARRAYSIZE(pickedObjectColor); ++i)
	{
		pickedObjectColor[i] = 0.0f;
	}
}

void InspectorWindow::showWindow()
{
	const ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	const ImVec2 maxWindowSize = ImVec2(static_cast<float>(Engine::getWindow()->getWidth()),
			static_cast<float>(Engine::getWindow()->getHeigth()));

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Inspector", &windowOpen, ImGuiWindowFlags_NoCollapse);
	{
		if (isObjectPicked)
		{
			{
				const float childOffsetTopY = 2.0f;
				const float nextNodeOffsetX = 18.0f;

				const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
						| ImGuiTreeNodeFlags_SpanAvailWidth;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childOffsetTopY);

				const ImVec2 childSize = ImVec2(ImGui::GetWindowWidth(), 40.0f);

				// color - #676767FF
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
					ImGui::Checkbox("##ShowPickedInspectorCheckbox", &showPickedObject);
					ImGui::PopItemWidth();

					ImGui::SameLine(115.0f);

					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
					ImGui::PushItemWidth(inputTextCustomWidth);

					ImGui::InputText("##ObjectNameEditor", pickedObjectName.data(), pickedObjectName.size());
					ImGui::PopStyleVar();
					ImGui::PopItemWidth();
				}
				ImGui::EndChild();

				ImGui::PopStyleColor();
			}

			if (ImGui::CollapsingHeader("Transform", componentsOpen[InspectorComponents::Transform]))
			{
				const float childObjectOffsetLeftX = 8.0f;
				const float inputFloatCustomWidth = 70.0f;

				const float inputFloatOffsetCoeff = 60.0f;
				const float inputSameLineOffset = ImGui::GetWindowWidth() - childObjectOffsetLeftX
						- inputFloatCustomWidth * 3.0f - inputFloatOffsetCoeff;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Position");
				ImGui::SameLine(inputSameLineOffset);

				// Transform position value windows
				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##PositionInspectorX", &positionVec3.x, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Y##PositionInspectorY", &positionVec3.y, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Z##PositionInspectorZ", &positionVec3.z, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Rotation");
				ImGui::SameLine(inputSameLineOffset);

				// Transform rotate value windows
				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##RotateInspectorX", &rotateVec3.x, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Y##RotateInspectorY", &rotateVec3.y, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Z##RotateInspectorZ", &rotateVec3.z, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Scale");
				ImGui::SameLine(inputSameLineOffset);

				// Transform scale value windows
				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##ScaleInspectorX", &rotateVec3.x, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Y##ScaleInspectorY", &rotateVec3.y, 0.0f, 0.0f, "%.2f");
					ImGui::SameLine();
					ImGui::InputFloat("Z##ScaleInspectorZ", &rotateVec3.z, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}
			}

			if (ImGui::CollapsingHeader("Color", componentsOpen[InspectorComponents::Color]))
			{
				const float childObjectOffsetLeftX = 70.0f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::ColorPicker4("color", pickedObjectColor);
			}

			if (ImGui::CollapsingHeader("Mesh", componentsOpen[InspectorComponents::Mesh]))
			{

			}

			if (ImGui::CollapsingHeader("Interaction", componentsOpen[InspectorComponents::Interaction]))
			{

			}

			if (ImGui::CollapsingHeader("Shader", componentsOpen[InspectorComponents::Shader]))
			{

			}

			showAddComponentBtn();
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

bool InspectorWindow::isWindowOpen() const
{
	return windowOpen;
}

void InspectorWindow::toggleWindow(const bool openWindow)
{
	windowOpen = openWindow;
}
void InspectorWindow::updateObjectPicked(const bool objectPicked)
{
	isObjectPicked = objectPicked;
}

void InspectorWindow::updateShowPickedObject(const bool showPicked)
{
	showPickedObject = showPicked;
}

void InspectorWindow::showAddComponentBtn()
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
			for (int i = 0; i < componentsNames.size(); ++i)
			{
				if (ImGui::Selectable(componentsNames[i].c_str()))
				{
					componentsOpen[i] = true;
				}
			}
			ImGui::EndPopup();
		}
	}
	ImGui::PopStyleVar();
}
}
