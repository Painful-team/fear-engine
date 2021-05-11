#include "Gui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

namespace FearEngine
{
// Main windows trackers
static bool ShowScene = true;
static bool ShowHierarchy = true;
static bool ShowProject = true;
static bool ShowInspector = true;
static bool ShowHelp = false;

// Scene window start, pause, stop trackers
static bool isSceneStarted = false;
static bool isScenePaused = false;

// Inspector components flags
static bool InspectorTransformComponent = true;
static bool InspectorColorComponent = true;
static bool InspectorMeshComponent = true;
static bool InspectorInteractionComponent = true;
static bool InspectorShaderComponent = true;

const int nInspectorComponents = 5;

// 0 - Transform, 1 - Color, 2 - Mesh, 3 - Interaction, 4 - Shader
static bool InspectorComponents[nInspectorComponents] = { true, true, true, true, true };
const char* ComponentsNames[nInspectorComponents] = { "Transform", "Color", "Mesh", "Interaction", "Shader" };

// Stubs for Inspector showing simple GUI logic
static bool ShowPickedInspectorObject = false;
static char ObjectInspectorName[32] = "Cube";

static float ObjectInspectorPositionX = 0.0f;
static float ObjectInspectorPositionY = 0.0f;
static float ObjectInspectorPositionZ = 0.0f;

static float ObjectInspectorRotationX = 0.0f;
static float ObjectInspectorRotationY = 0.0f;
static float ObjectInspectorRotationZ = 0.0f;

static float ObjectInspectorScaleX = 0.0f;
static float ObjectInspectorScaleY = 0.0f;
static float ObjectInspectorScaleZ = 0.0f;

static float ObjectInspectorColor[4] = { 0.4f, 0.7f, 0.0f, 0.5f };

void Gui::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

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
	}
}

FearEngine::Gui::~Gui()
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

	if (ShowScene)			showSceneWindow(&ShowScene);
	if (ShowHierarchy)	showHierarchyWindow(&ShowHierarchy);
	if (ShowProject)		showProjectWindow(&ShowProject);
	if (ShowInspector)	showInspectorWindow(&ShowInspector, true);
	if (ShowHelp)				showHelpWindow(&ShowHelp);

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
	ImFont* mainMenuFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("./resources/fonts/Roboto-Regular.ttf", 20.0f);
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
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	const ImVec2 dockingAreaSize = ImVec2(viewport->GetWorkSize().x, viewport->GetWorkSize().y - bottomPanelHeight);

	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(dockingAreaSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("docker window", &enableDocking, windowFlags);

	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	ImGui::End();
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
				ShowScene = true;
			}
			if (ImGui::MenuItem("Project"))
			{
				ShowProject = true;
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				ShowHierarchy = true;
			}
			if (ImGui::MenuItem("Inspector"))
			{
				ShowInspector = true;
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
				ShowHelp = true;
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

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // #FFFFFFFF

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

void Gui::showSceneWindow(bool* windowOpen)
{
	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	ImVec2 maxWindowSize = ImVec2(Engine::getWindow()->getWidth(), Engine::getWindow()->getHeigth());

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Scene", windowOpen, windowFlags);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Stats"))
			{
				showStatsDialog();
				ImGui::EndMenu();
			}

			{
				const float btnOffset = 5.0f;
				const ImVec2 btnSize = ImVec2(24.0f, 24.0f);

				const ImVec4 btnOnColor = ImVec4(0.227, 0.227, 0.227, 1.0); // color - #3A3A3AFF
				const ImVec4 btnOffColor = ImVec4(0.151, 0.157, 0.157, 1.0); // color - #292828FF

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f - btnSize.x - btnOffset);

				if (isSceneStarted && !isScenePaused)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOffColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOnColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				if (ImGui::Button("|>", btnSize))
				{
					// Start scene
					isSceneStarted = true;
					isScenePaused = false;
				}

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f);

				if (!isSceneStarted || isScenePaused)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOffColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOnColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				if (ImGui::Button("||", btnSize))
				{
					// Pause scene
					isScenePaused = true;
				}

				ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f + btnSize.x + btnOffset);

				if (!isSceneStarted)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOffColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, btnOnColor);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				if (ImGui::Button("|=|", btnSize))
				{
					// Stop scene
					isSceneStarted = false;
					isScenePaused = false;
				}
				ImGui::PopStyleColor(3);

				ImGui::PopItemFlag();
				ImGui::PopItemFlag();
				ImGui::PopItemFlag();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	ImGui::PopStyleVar();
}

void Gui::showHierarchyWindow(bool* windowOpen)
{
	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	ImVec2 maxWindowSize = ImVec2(Engine::getWindow()->getWidth(), Engine::getWindow()->getHeigth());

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (ImGui::Begin("Hierarchy", windowOpen, windowFlags))
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

		ImVec2 childSize = ImVec2(ImGui::GetWindowWidth() - childOffsetX, ImGui::GetWindowHeight() - childOffsetBottomY);
		ImGuiWindowFlags childFlags = ImGuiWindowFlags_NoScrollbar;
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childOffsetX);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childOffsetTopY);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

		if (ImGui::BeginChild("Hierarchy area", childSize, false, childFlags))
		{
			const int nCubes = 200;

			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(67, 67, 67, 100));

			if (ImGui::CollapsingHeader("Scene name"))
			{
				static int selection_mask = (1 << 2);
				int nodeClicked = -1;

				for (int i = 0; i < nCubes; i++)
				{
					ImGuiTreeNodeFlags nodeFlags = treeFlags;
					const bool isSelected = (selection_mask & (1 << i)) != 0;
					bool nodeOpen = false;

					if (isSelected)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
					}

					if (i < 199)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
						ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, "Cube - %d", i + 1);
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

						nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, "Collection");
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
							ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, "Cube - %d", i);
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
					if (ImGui::GetIO().KeyCtrl)
					{
						selection_mask ^= (1 << nodeClicked);
					}
					else
					{
						selection_mask = (1 << nodeClicked);
					}
				}
				else if (ImGui::IsMouseClicked(0))
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

void Gui::showProjectWindow(bool* windowOpen)
{
	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

	ImVec2 minWindowSize = ImVec2(200.0f, 200.0f);
	ImVec2 maxWindowSize = ImVec2(ImVec2(Engine::getWindow()->getWidth(), Engine::getWindow()->getHeigth()));

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (ImGui::Begin("Project", windowOpen, windowFlags))
	{
		const float mainAreaWidthX = 175.0f;
		const float barAreaOffsetLeftX = 3.0f;
		const float barAreaWidthX = ImGui::GetWindowWidth() - mainAreaWidthX - barAreaOffsetLeftX;
		const float mainAreaOffsetBottomY = 55.0f;
		const float barAreaHeight = 26.0f;

		const float startPosY = ImGui::GetCursorPosY();

		ImVec2 fileMainAreaSize = ImVec2(mainAreaWidthX, ImGui::GetWindowHeight() - mainAreaOffsetBottomY);

		ImVec2 fileBarAreaSize = ImVec2(barAreaWidthX, barAreaHeight);

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

		// File main (left) area
		{
			const float childOffsetTopY = 2.0f;
			const float nextNodeOffsetX = 18.0f;

			ImGuiWindowFlags childFlags = ImGuiWindowFlags_NoScrollbar;
			ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_SpanAvailWidth;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX());
			ImGui::SetCursorPosY(startPosY + childOffsetTopY);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

			if (ImGui::BeginChild("FileMainArea", fileMainAreaSize, false, childFlags))
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

			ImGuiWindowFlags childFlags = ImGuiWindowFlags_NoScrollbar;
			ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_SpanAvailWidth;

			ImGui::SetCursorPosX(mainAreaWidthX + barAreaOffsetLeftX);
			ImGui::SetCursorPosY(startPosY + childOffsetTopY);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(67, 67, 67, 100));

			if (ImGui::BeginChild("FileBarArea", fileBarAreaSize, false, childFlags))
			{
				std::string text = "Assets >\0";

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

void Gui::showInspectorWindow(bool* windowOpen, bool isObjectPicked = false)
{
	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoCollapse;

	ImVec2 minWindowSize = ImVec2(410.0f, 250.0f);
	ImVec2 maxWindowSize = ImVec2(ImVec2(Engine::getWindow()->getWidth(), Engine::getWindow()->getHeigth()));

	ImGui::SetNextWindowSizeConstraints(minWindowSize, maxWindowSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Inspector", windowOpen, windowFlags);
	{
		if (isObjectPicked)
		{
			{
				const float childOffsetTopY = 2.0f;
				const float nextNodeOffsetX = 18.0f;

				ImGuiWindowFlags childFlags = ImGuiWindowFlags_NoScrollbar;
				ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
						| ImGuiTreeNodeFlags_SpanAvailWidth;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childOffsetTopY);

				ImVec2 childSize = ImVec2(ImGui::GetWindowWidth(), 40.0f);

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
					ImGui::Checkbox("", &ShowPickedInspectorObject);
					ImGui::PopItemWidth();

					ImGui::SameLine(115.0f);

					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
					ImGui::PushItemWidth(inputTextCustomWidth);

					ImGui::InputText("##edit", ObjectInspectorName, IM_ARRAYSIZE(ObjectInspectorName));

					ImGui::PopStyleVar();
					ImGui::PopItemWidth();
				}
				ImGui::EndChild();

				ImGui::PopStyleColor();
			}

			if (ImGui::CollapsingHeader("Transform", &InspectorComponents[0]))
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
					ImGui::InputFloat("X##PositionX", &ObjectInspectorPositionX, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();
					ImGui::InputFloat("Y##PositionY", &ObjectInspectorPositionY, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();
					ImGui::InputFloat("Z##PositionZ", &ObjectInspectorPositionZ, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Rotation");
				ImGui::SameLine(inputSameLineOffset);

				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##RotateX", &ObjectInspectorRotationX, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();
					ImGui::InputFloat("Y##RotateY", &ObjectInspectorRotationY, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();
					ImGui::InputFloat("Z##RotateZ", &ObjectInspectorRotationZ, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::Text("Scale");
				ImGui::SameLine(inputSameLineOffset);

				{
					ImGui::PushItemWidth(inputFloatCustomWidth);
					ImGui::InputFloat("X##ScaleX", &ObjectInspectorScaleX, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();
					ImGui::InputFloat("Y##ScaleY", &ObjectInspectorScaleY, 0.0f, 0.0f, "%.2f"); ImGui::SameLine();
					ImGui::InputFloat("Z##ScaleZ", &ObjectInspectorScaleZ, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
				}
			}

			if (ImGui::CollapsingHeader("Color", &InspectorComponents[1]))
			{
				const float childObjectOffsetLeftX = 70.0f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + childObjectOffsetLeftX);

				ImGui::ColorPicker4("color", ObjectInspectorColor);
			}

			if (ImGui::CollapsingHeader("Mesh", &InspectorComponents[2]))
			{

			}

			if (ImGui::CollapsingHeader("Interaction", &InspectorComponents[3]))
			{

			}

			if (ImGui::CollapsingHeader("Shader", &InspectorComponents[4]))
			{

			}

			{
				const float btnOffsetRightX = 75.0f;
				const float btnOffsetTopY = 15.0f;

				ImVec2 addComponentBtnSize = ImVec2(150.0f, 35.0f);

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() / 2.0f - btnOffsetRightX);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + btnOffsetTopY);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 250.0f);
				ImGui::Button("Add component", addComponentBtnSize);
				{
					if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonLeft))
					{
						for (int i = 0; i < IM_ARRAYSIZE(ComponentsNames); i++)
						{
							if (ImGui::Selectable(ComponentsNames[i]))
							{
								InspectorComponents[i] = true;
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

void Gui::showHelpWindow(bool* windowOpen)
{
	ImGuiWindowFlags windowFlags = 0;

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImVec2 popupSize = ImVec2(500.0f, 300.0f);

	ImGui::OpenPopup("Help");

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(popupSize);

	if (ImGui::BeginPopupModal("Help", windowOpen, windowFlags))
	{
		const float buttonOffsetBottomY = 35.0f;

		ImGui::Text("There should be some helping guide ^_*");

		ImGui::SetCursorPosY(popupSize.y - buttonOffsetBottomY);
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			*windowOpen = false;
		}

		ImGui::SameLine(popupSize.x);

		if (ImGui::Button("Next"))
		{
		}

		ImGui::EndPopup();
	}
}

void Gui::showStatsDialog()
{
	ImGuiWindowFlags childFlags = 0;

	childFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::BeginChild("child", ImVec2(263.0f, 152.0f), false, childFlags);
	{
		ImGui::Text("Polygons: %d");
		ImGui::Text("Objects: %d");
		ImGui::Text("Screen: %d");
	}
	ImGui::EndChild();
}
}
