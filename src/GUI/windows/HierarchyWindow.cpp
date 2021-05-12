#include "HierarchyWindow.hpp"

#include <core/Input.hpp>

namespace FearEngine::UI::windows
{
HierarchyWindow::HierarchyWindow() :
	isWindowOpen_(true)
{}

void HierarchyWindow::showWindow(const WindowSettings* settings)
{
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

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
	if (ImGui::Begin("Hierarchy", &isWindowOpen_, windowFlags))
	{
		const float childOffsetX = 32.0f;
		const float childOffsetBottomY = 55.0f;
		const float childOffsetTopY = 2.0f;

		const float nextNodeOffsetX = 18.0f;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				// #TODO: Implement '+' to adding new figures in Hierarchy and scene

				ImGui::EndMenu();
			}

			{
				ImGuiTextFilter filter;
				const float filterWidth = 160.0f;

				const float addMenuItemWidth = 40.0f;
				const float filterRightOffset = 125.0f;

				const float filterSameLineOffset = ImGui::GetWindowWidth() - addMenuItemWidth - filterRightOffset;

				ImGui::SameLine(filterSameLineOffset);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);
				filter.Draw("", filterWidth);
				ImGui::PopStyleVar();
			}

			ImGui::EndMenuBar();
		}

		const ImVec2 childSize = ImVec2(ImGui::GetWindowWidth() - childOffsetX,
				ImGui::GetWindowHeight() - childOffsetBottomY);
		const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childOffsetX);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childOffsetTopY);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

		if (ImGui::BeginChild("Hierarchy area", childSize, false, ImGuiWindowFlags_NoScrollbar))
		{
			const int nCubes = 200;

			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(67, 67, 67, 100));

			if (ImGui::CollapsingHeader("Scene name"))
			{
				static int selection_mask = (1 << 2);
				int nodeClicked = -1;

				for (int i = 0; i < nCubes; ++i)
				{
					ImGuiTreeNodeFlags nodeFlags = treeFlags;
					const bool isSelected = (selection_mask & (1 << i)) != 0;
					bool nodeOpen = false;

					if (isSelected)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
					}

					if (i < nCubes - 1)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
						ImGui::TreeNodeEx(static_cast<void*>(&i), nodeFlags, "Cube - %d", i + 1);
						if (ImGui::IsItemClicked())
						{
							nodeClicked = i;
						}
						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
							ImGui::Text("drag and drop");
							ImGui::EndDragDropSource();
						}
					}
					else
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + nextNodeOffsetX);

						nodeFlags |= ImGuiTreeNodeFlags_SpanFullWidth;

						nodeOpen = ImGui::TreeNodeEx(static_cast<void*>(&i), nodeFlags, "Collection");
						if (ImGui::IsItemClicked())
						{
							nodeClicked = i;
						}
						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
							ImGui::Text("drag and drop");
							ImGui::EndDragDropSource();
						}
						if (nodeOpen)
						{
							if ((selection_mask & (1 << ++i)) != 0)
							{
								nodeFlags |= ImGuiTreeNodeFlags_Selected;
							}

							nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
							ImGui::TreeNodeEx(static_cast<void*>(&i), nodeFlags, "Cube - %d", i);
							if (ImGui::IsItemClicked())
							{
								nodeClicked = i;
							}
							if (ImGui::BeginDragDropSource())
							{
								ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
								ImGui::Text("drag and drop");
								ImGui::EndDragDropSource();
							}
							ImGui::TreePop();
						}
					}
				}
				if (nodeClicked != -1)
				{
					
					if (FearEngine::Input::isKeyPressed(FearEngine::Events::Key::LEFT_CONTROL))
					{
						selection_mask ^= (1 << nodeClicked);
					}
					else
					{
						selection_mask = (1 << nodeClicked);
					}
				}
				else if (FearEngine::Input::isMousePressed(FearEngine::Events::Mouse::BUTTON_LEFT))
				{
					selection_mask = (1 << 8);
				}
			}
			ImGui::PopStyleColor();
		}
		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	ImGui::End();

	ImGui::PopStyleVar();
}

bool HierarchyWindow::isWindowOpen() const
{
	return isWindowOpen_;
}

void HierarchyWindow::toggleWindow(const bool openWindow)
{
	isWindowOpen_ = openWindow;
}
}
