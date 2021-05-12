#include "Gui.hpp"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <event/GuiEvent.hpp>

namespace FearEngine
{
void Gui::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	static_cast<void>(io);

	ImGui_ImplGlfw_InitForOpenGL(Engine::getWindow()->window, true);
	ImGui_ImplOpenGL3_Init();

	applyInitialSettings();
}

Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::onGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	showMainMenuBar();
	windows.showAllWindows();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	Engine::getDispatcher()->notify(&Events::GuiUpdate());
}

void Gui::applyInitialSettings()
{
	setWindowStyles();
	setMainWindowFlags();
	setFonts();
	setMainColors();
}

void Gui::setWindowStyles()
{
	ImGuiStyle& ImGuiMainStyle = ImGui::GetStyle();

	ImGuiMainStyle.WindowMenuButtonPosition = ImGuiDir_None;

	ImGuiMainStyle.ChildBorderSize = 0.0f;

	ImGuiMainStyle.WindowRounding = 0.0f;
	ImGuiMainStyle.ChildRounding = 0.0f;
	ImGuiMainStyle.FrameRounding = 0.0f;
	ImGuiMainStyle.GrabRounding = 0.0f;
	ImGuiMainStyle.PopupRounding = 0.0f;
	ImGuiMainStyle.ScrollbarRounding = 12.0f;
	ImGuiMainStyle.TabRounding = 0.0f;
}

void Gui::setMainWindowFlags()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::GetIO().ConfigDockingAlwaysTabBar = true;
}

void Gui::setFonts()
{
	const ImFont* mainMenuFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("./resources/fonts/Roboto-Regular.ttf", 20.0f);
	IM_ASSERT(mainMenuFont != NULL);
}

void Gui::setMainColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	// color - #F2F2F2FF
	colors[ImGuiCol_Text]								= ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	// color - #4A4A4AFF
	colors[ImGuiCol_TextDisabled]						= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_WindowBg]							= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #262626FF
	colors[ImGuiCol_ChildBg]							= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	// color - #262626FF
	colors[ImGuiCol_PopupBg]							= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	// color - #000000FF
	colors[ImGuiCol_Border]								= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	// color - #000000FF
	colors[ImGuiCol_BorderShadow]						= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	// color - #1F1F1FFF
	colors[ImGuiCol_FrameBg]							= ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_FrameBgHovered]						= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #616161FF
	colors[ImGuiCol_FrameBgActive]						= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_TitleBg]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_TitleBgActive]						= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_TitleBgCollapsed]					= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_MenuBarBg]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #0F0F0FFF
	colors[ImGuiCol_ScrollbarBg]						= ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_ScrollbarGrab]						= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #4F4F4FFF
	colors[ImGuiCol_ScrollbarGrabHovered]				= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	// color - #5E5E5EFF
	colors[ImGuiCol_ScrollbarGrabActive]				= ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	// color - #7A7A7AFF
	colors[ImGuiCol_CheckMark]							= ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_SliderGrab]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #7A7A7AFF
	colors[ImGuiCol_SliderGrabActive]					= ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_Button]								= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #525252FF
	colors[ImGuiCol_ButtonHovered]						= ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	// color - #2B5C87FF
	colors[ImGuiCol_ButtonActive]						= ImVec4(0.17f, 0.36f, 0.53f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_Header]								= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #4A4A4AFF
	colors[ImGuiCol_HeaderHovered]						= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	// color - #575757FF
	colors[ImGuiCol_HeaderActive]						= ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
	// color - #0A0A0AFF
	colors[ImGuiCol_Separator]							= ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	// color - #0A0A0AFF
	colors[ImGuiCol_SeparatorHovered]					= ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	// color - #0A0A0AFF
	colors[ImGuiCol_SeparatorActive]					= ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_ResizeGrip]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #545454FF
	colors[ImGuiCol_ResizeGripHovered]					= ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	// color - #616161FF
	colors[ImGuiCol_ResizeGripActive]					= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_Tab]								= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #545454FF
	colors[ImGuiCol_TabHovered]							= ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	// color - #636363FF
	colors[ImGuiCol_TabActive]							= ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_TabUnfocused]						= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_TabUnfocusedActive]					= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #2B5C87FF
	colors[ImGuiCol_DockingPreview]						= ImVec4(0.17f, 0.36f, 0.53f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_DockingEmptyBg]						= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #5E5E5EFF
	colors[ImGuiCol_PlotLines]							= ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	// color - #4296FA59
	colors[ImGuiCol_TextSelectedBg]						= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	// color - #FFFF00E6
	colors[ImGuiCol_DragDropTarget]						= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	// color - #4296FAFF
	colors[ImGuiCol_NavHighlight]						= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	// color - #F2F2F2FF
	colors[ImGuiCol_NavWindowingHighlight]				= ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	// color - #CCCCCC33
	colors[ImGuiCol_NavWindowingDimBg]					= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	// color - #CCCCCC59
	colors[ImGuiCol_ModalWindowDimBg]					= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Gui::showMainMenuBar()
{
	// Set menu bar background color - #4F4F4FFF
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.31f, 0.31f, 0.31f, 1.0f));

	// Set menu bar height and width
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 5.0f));

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Project...")) {}
				if (ImGui::MenuItem("Scene...")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Open"))
			{
				if (ImGui::MenuItem("Project...")) {}
				if (ImGui::MenuItem("Scene...")) {}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item

			ImGui::Separator();

			if (ImGui::MenuItem("Select all")) {}
			if (ImGui::MenuItem("Deselect all")) {}

			ImGui::Separator();

			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Scene"))
			{
				windows.sceneWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Project"))
			{
				windows.projectWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				windows.hierarchyWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Inspector"))
			{
				windows.inspectorWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Debug (console)"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Guide"))
			{
				windows.helpWindow.toggleWindow(true);
			}

			ImGui::EndMenu();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndMainMenuBar();
	}
}
}
