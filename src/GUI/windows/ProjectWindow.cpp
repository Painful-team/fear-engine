#include "ProjectWindow.hpp"

#include <core/Engine.hpp>

namespace FearEngine::UI::windows
{
ProjectWindow::ProjectWindow():
	windowOpen(true),
	filterItemEnabled(true)
{}

void ProjectWindow::showWindow()
{
	const ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	const ImVec2 maxWindowSize = ImVec2(static_cast<float>(Engine::getWindow()->getWidth()),
			static_cast<float>(Engine::getWindow()->getHeight()));

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Project", &windowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
	{
		const float mainAreaWidthX = 175.0f;
		const float barAreaOffsetLeftX = 3.0f;
		const float barAreaWidthX = ImGui::GetWindowWidth() - mainAreaWidthX - barAreaOffsetLeftX;
		const float mainAreaOffsetBottomY = 55.0f;
		const float barAreaHeight = 26.0f;

		const float startPosY = ImGui::GetCursorPosY();

		const ImVec2 fileMainAreaSize = ImVec2(mainAreaWidthX, ImGui::GetWindowHeight() - mainAreaOffsetBottomY);
		const ImVec2 fileBarAreaSize = ImVec2(barAreaWidthX, barAreaHeight);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Add"))
			{
				ImGui::EndMenu();
			}

			showFilterMenuItem();

			ImGui::EndMenuBar();
		}

		// File main (left) area
		{
			const float childOffsetTopY = 2.0f;
			const float nextNodeOffsetX = 18.0f;

			ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
					| ImGuiTreeNodeFlags_SpanAvailWidth;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			ImGui::SetCursorPosY(startPosY + childOffsetTopY);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

			if (ImGui::BeginChild("FileMainArea", fileMainAreaSize, false, ImGuiWindowFlags_NoScrollbar))
			{
				const int nElems = 2;

				ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(67, 67, 67, 100));

				if (ImGui::CollapsingHeader("Assets"))
				{

				}
				ImGui::PopStyleColor();
			}
			ImGui::EndChild();

			ImGui::PopStyleColor();
		}

		// File bar (top) area
		{
			const float childOffsetTopY = 2.0f;

			const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
					| ImGuiTreeNodeFlags_SpanAvailWidth;

			ImGui::SetCursorPosX(mainAreaWidthX + barAreaOffsetLeftX);
			ImGui::SetCursorPosY(startPosY + childOffsetTopY);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

			if (ImGui::BeginChild("FileBarArea", fileBarAreaSize, false, ImGuiWindowFlags_NoScrollbar))
			{
				const std::string text = "Assets >";

				const float startTextOffsetX = 3.0f;
				const float startTextOffsetY = 3.0f;

				ImGui::SetCursorPosX(startTextOffsetX);
				ImGui::SetCursorPosY(startTextOffsetY);

				ImGui::Text(text.c_str());

				// #TODO: Implement file header system
			}
			ImGui::EndChild();

			ImGui::PopStyleColor();
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

bool ProjectWindow::isWindowOpen() const
{
	return windowOpen;
}

void ProjectWindow::toggleWindow(const bool openWindow)
{
	windowOpen = openWindow;
}

bool ProjectWindow::isFilterEnabled() const
{
	return filterItemEnabled;
}

void ProjectWindow::toggleFilterItem(const bool enableFilter)
{
	filterItemEnabled = enableFilter;
}

void ProjectWindow::showFilterMenuItem() const
{
	if (filterItemEnabled)
	{
		ImGuiTextFilter filter;
		const float filterWidth = 160.0f;

		const float addMenuItemWidth = 40.0f;
		const float filterRightOffset = 125.0f;

		const float filterSameLineOffset = ImGui::GetWindowWidth() - addMenuItemWidth - filterRightOffset;

		ImGui::SameLine(filterSameLineOffset);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);
		filter.Draw("FilterMenuItemProject", filterWidth);
		ImGui::PopStyleVar();
	}
}
}
