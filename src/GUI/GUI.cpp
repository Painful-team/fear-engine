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
}

void Gui::run()
{
	applyInitialSettings();

	while (!glfwWindowShouldClose(Engine::getWindow()->window))
	{
		glfwPollEvents();

		onGui();

		Engine::getDispatcher()->notify(&Events::GuiUpdate());
	}
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

	showDockingArea();
	showMainMenuBar();
	showBottomPanel();

	windows_.showAllWindows();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

	colors[ImGuiCol_Text]										= ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_TextDisabled]						= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_WindowBg]								= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ChildBg]								= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_PopupBg]								= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_Border]									= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_BorderShadow]						= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_FrameBg]								= ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]					= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_FrameBgActive]					= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_TitleBg]								= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgActive]					= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]				= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_MenuBarBg]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]						= ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab]					= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_CheckMark]							= ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	colors[ImGuiCol_SliderGrab]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]				= ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	colors[ImGuiCol_Button]									= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonHovered]					= ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_ButtonActive]						= ImVec4(0.17f, 0.36f, 0.53f, 1.00f);
	colors[ImGuiCol_Header]									= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_HeaderHovered]					= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_HeaderActive]						= ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
	colors[ImGuiCol_Separator]							= ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_SeparatorHovered]				= ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_SeparatorActive]				= ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_ResizeGrip]							= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered]			= ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_ResizeGripActive]				= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab]										= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_TabHovered]							= ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabActive]							= ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_TabUnfocused]						= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive]			= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_DockingPreview]					= ImVec4(0.17f, 0.36f, 0.53f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg]					= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines]							= ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]				= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]					= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]					= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]					= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]						= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_NavWindowingDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]				= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Gui::showDockingArea()
{
	bool enableDocking = true;
	const float bottomPanelHeight = 30.0f;

	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
			| ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	const ImVec2 dockingAreaSize = ImVec2(viewport->GetWorkSize().x, viewport->GetWorkSize().y - bottomPanelHeight);

	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(dockingAreaSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("docker window", &enableDocking, windowFlags);

	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	ImGui::End();
}

void Gui::showMainMenuBar()
{
	// color - #4F4F4FFF
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.31f, 0.31f, 0.31f, 1.0f));

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
			// Disabled item
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}

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
				windows_.sceneWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Project"))
			{
				windows_.projectWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				windows_.hierarchyWindow.toggleWindow(true);
			}
			if (ImGui::MenuItem("Inspector"))
			{
				windows_.inspectorWindow.toggleWindow(true);
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
				windows_.helpWindow.toggleWindow(true);
			}

			ImGui::EndMenu();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndMainMenuBar();
	}
}

void Gui::showBottomPanel()
{
	bool showBottomPanel = true;
	const float panelHeight = 30.0f;

	ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(mainViewport->GetWorkPos().x, static_cast<float>(Engine::getWindow()->getHeigth())
			- panelHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(Engine::getWindow()->getWidth()), panelHeight), ImGuiCond_Always);

	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoInputs;

	// color - #FFFFFFFF
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	ImGui::Begin("Bottom panel", &showBottomPanel, windowFlags);
	{
		std::string text = "dev";

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
				- 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);

		ImGui::Text(text.c_str());
	}
	ImGui::End();

	ImGui::PopStyleColor();
}
}
