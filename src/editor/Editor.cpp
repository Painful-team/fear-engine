#include "Editor.hpp"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <core/Engine.hpp>
#include <event/GuiEvent.hpp>

#include <core/Input.hpp>

#include <ImGuizmo/ImGuizmo.h>

namespace FearEngine
{
Editor::Editor()
 : mouseReq(false)
{}
int Editor::init()
{
	windows.init();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	Engine::getDispatcher()->get<Events::MouseMoved>()->attach<&Editor::onMouseMoved>(this);
	Engine::getDispatcher()->get<Events::MouseButtonPressed>()->attach<&Editor::onMousePressed>(this);
	Engine::getDispatcher()->get<Events::MouseButtonReleased>()->attach<&Editor::onMouseReleased>(this);
	Engine::getDispatcher()->get<Events::MouseScrolled>()->attach<&Editor::onScroll>(this);
	Engine::getDispatcher()->get<Events::KeyPressed>()->attach<&Editor::onKeyPressed>(this);
	Engine::getDispatcher()->get<Events::KeyReleased>()->attach<&Editor::onKeyReleased>(this);
	Engine::getDispatcher()->get<Events::KeyTyped>()->attach<&Editor::onKeyTyped>(this);
	Engine::getDispatcher()->get<Events::MouseRequired>()->attach<&Editor::onMouseRequired>(this);

	ImGui_ImplGlfw_InitForOpenGL(Engine::getWindow()->window, false);
	ImGui_ImplOpenGL3_Init();

	applyInitialSettings();

	return 0;
}

void Editor::resize(int width, int height) {}

Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Editor::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	windows.dockingArea.showWindow();

	showMainMenuBar();
	windows.showAllWindows();
}

void Editor::end()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Editor::onMouseMoved(Events::MouseMoved* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e->getX(), e->getY());

	if (windows.sceneWindow.isFocused())
	{
		return true;
	}

	return false;
}

bool Editor::onMousePressed(Events::MouseButtonPressed* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(e->getButton(), true);

	if (windows.sceneWindow.viewPorts[0].isFocused())
	{
		if (e->getButton() == Events::Mouse::BUTTON_LEFT && !ImGuizmo::IsOver())
		{
			auto pos = Input::getMousePos();

			auto& comp = windows.sceneWindow.editorCamera.getComponent<Component::Camera>();
			pos -= windows.sceneWindow.viewPorts[0].contentRegion[0];

			glm::vec2 viewportSize = windows.sceneWindow.viewPorts[0].contentRegion[1] - windows.sceneWindow.viewPorts[0].contentRegion[0];
			pos.y = viewportSize.y - pos.y;

			auto data = comp.getFrameBuffer().getPixel(Render::FrameBufferType::ColorAttachment1, pos);
			char udata[4]{static_cast<char>(data.x), static_cast<char>(data.y), static_cast<char>(data.z), static_cast<char>(data.w)};
			int EntityNum = *reinterpret_cast<uint32_t*>(udata);

			if (EntityNum != -1)
			{
				windows.inspectorWindow.chosenEntity = Engine::getScene()->getEntity(EntityNum);
			}
			else
			{
				windows.inspectorWindow.chosenEntity = Entity{};
			}
		}

		return true;
	}

	return false;
}

bool Editor::onMouseReleased(Events::MouseButtonReleased* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(e->getButton(), false);

	if (windows.sceneWindow.isFocused())
	{
		return true;
	}

	if (!windows.sceneWindow.isFocused() && Engine::getWindow()->isCursorBlocked())
	{
		Engine::getWindow()->unblockCursor();
	}

	return false;
}

bool Editor::onMouseRequired(Events::MouseRequired* e)
{
	mouseReq = e->isRequired();
	return true;
}

bool Editor::onScroll(Events::MouseScrolled* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(e->getXoffset(), e->getYoffset());

	if (windows.sceneWindow.isFocused())
	{
		return true;
	}

	return false;
}

ImGuiKey translateKeyToImGui(FearEngine::Events::keys key);

bool Editor::onKeyPressed(Events::KeyPressed* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(translateKeyToImGui(e->keyCode()), true);

	if (windows.sceneWindow.isFocused())
	{
		if (!windows.inspectorWindow.chosenEntity.isValid())
		{
			return true;
		}

		if (e->keyCode() == Events::Key::E)
		{
			windows.sceneWindow.gizmoOperation = ImGuizmo::OPERATION::SCALE;
		}

		if (e->keyCode() == Events::Key::R)
		{
			windows.sceneWindow.gizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}

		if (e->keyCode() == Events::Key::T)
		{
			windows.sceneWindow.gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}

		return true;
	}

	return false;
}

bool Editor::onKeyReleased(Events::KeyReleased* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(translateKeyToImGui(e->keyCode()), false);

	if (windows.sceneWindow.isFocused())
	{
		return true;
	}

	return false;
}

bool Editor::onKeyTyped(Events::KeyTyped* e)
{
	if (mouseReq)
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();

	if (windows.sceneWindow.isFocused())
	{
		return true;
	}

	io.AddInputCharacter(e->keyCode());

	return false;
}

bool Editor::onResize(Events::WindowResize* e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(e->getWidth(), e->getHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	if (windows.sceneWindow.isFocused())
	{
		return true;
	}

	return false;
}

void Editor::applyInitialSettings()
{
	setWindowStyles();
	setFonts();
	setMainColors();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;
}

void Editor::setWindowStyles()
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

void Editor::setFonts()
{
	const ImFont* mainMenuFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Regular.ttf", 20.0f);
	IM_ASSERT(mainMenuFont != NULL);
}

void Editor::setMainColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	// color - #F2F2F2FF
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	// color - #4A4A4AFF
	colors[ImGuiCol_TextDisabled] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #262626FF
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	// color - #262626FF
	colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	// color - #000000FF
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	// color - #000000FF
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	// color - #1F1F1FFF
	colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #616161FF
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #333333FF
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #0F0F0FFF
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #4F4F4FFF
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	// color - #5E5E5EFF
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	// color - #7A7A7AFF
	colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #7A7A7AFF
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #525252FF
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	// color - #2B5C87FF
	colors[ImGuiCol_ButtonActive] = ImVec4(0.17f, 0.36f, 0.53f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #4A4A4AFF
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	// color - #575757FF
	colors[ImGuiCol_HeaderActive] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
	// color - #0A0A0AFF
	colors[ImGuiCol_Separator] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	// color - #0A0A0AFF
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	// color - #0A0A0AFF
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #545454FF
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	// color - #616161FF
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #545454FF
	colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	// color - #636363FF
	colors[ImGuiCol_TabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #424242FF
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	// color - #5E5E5EFF
	colors[ImGuiCol_PlotLines] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	// color - #4296FA59
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	// color - #FFFF00E6
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	// color - #4296FAFF
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	// color - #F2F2F2FF
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	// color - #CCCCCC33
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	// color - #CCCCCC59
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Editor::showMainMenuBar()
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
				if (ImGui::MenuItem("Project..."))
				{}
				if (ImGui::MenuItem("Scene..."))
				{}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Open"))
			{
				if (ImGui::MenuItem("Project..."))
				{}
				if (ImGui::MenuItem("Scene..."))
				{}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{}
			if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
			{}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
			{}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
			{}

			ImGui::Separator();

			if (ImGui::MenuItem("Select all"))
			{}
			if (ImGui::MenuItem("Deselect all"))
			{}

			ImGui::Separator();

			if (ImGui::MenuItem("Cut", "CTRL+X"))
			{}
			if (ImGui::MenuItem("Copy", "CTRL+C"))
			{}
			if (ImGui::MenuItem("Paste", "CTRL+V"))
			{}

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
			{}

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

void EditorMainWindows::init()
{
	sceneWindow.init();

	hierarchyWindow.init();
	projectWindow.init();
	inspectorWindow.init();
	helpWindow.init();

	bottomPanel.init();
	dockingArea.init();
}

void EditorMainWindows::showAllWindows()
{
	if (sceneWindow.isWindowOpen())
	{
		sceneWindow.showWindow();
	}

	if (hierarchyWindow.isWindowOpen())
	{
		hierarchyWindow.showWindow();
	}

	if (projectWindow.isWindowOpen())
	{
		projectWindow.showWindow();
	}

	if (inspectorWindow.isWindowOpen())
	{
		inspectorWindow.showWindow();
	}

	if (helpWindow.isWindowOpen())
	{
		helpWindow.showWindow();
	}

	if (bottomPanel.isPanelEnabled())
	{
		bottomPanel.showWindow();
	}
}

ImGuiKey translateKeyToImGui(FearEngine::Events::keys key)
{
	using namespace FearEngine::Events;
	switch (key)
	{
	case Key::TAB:
		return ImGuiKey_Tab;
	case Key::LEFT:
		return ImGuiKey_LeftArrow;
	case Key::RIGHT:
		return ImGuiKey_RightArrow;
	case Key::UP:
		return ImGuiKey_UpArrow;
	case Key::DOWN:
		return ImGuiKey_DownArrow;
	case Key::PAGE_UP:
		return ImGuiKey_PageUp;
	case Key::PAGE_DOWN:
		return ImGuiKey_PageDown;
	case Key::HOME:
		return ImGuiKey_Home;
	case Key::END:
		return ImGuiKey_End;
	case Key::INSERT:
		return ImGuiKey_Insert;
	case Key::DEL:
		return ImGuiKey_Delete;
	case Key::BACKSPACE:
		return ImGuiKey_Backspace;
	case Key::SPACE:
		return ImGuiKey_Space;
	case Key::ENTER:
		return ImGuiKey_Enter;
	case Key::ESCAPE:
		return ImGuiKey_Escape;
	case Key::APOSTROPHE:
		return ImGuiKey_Apostrophe;
	case Key::COMMA:
		return ImGuiKey_Comma;
	case Key::MINUS:
		return ImGuiKey_Minus;
	case Key::PERIOD:
		return ImGuiKey_Period;
	case Key::SLASH:
		return ImGuiKey_Slash;
	case Key::SEMICOLON:
		return ImGuiKey_Semicolon;
	case Key::EQUAL:
		return ImGuiKey_Equal;
	case Key::LEFT_BRACKET:
		return ImGuiKey_LeftBracket;
	case Key::BACKSLASH:
		return ImGuiKey_Backslash;
	case Key::RIGHT_BRACKET:
		return ImGuiKey_RightBracket;
	case Key::GRAVE_ACCENT:
		return ImGuiKey_GraveAccent;
	case Key::CAPS_LOCK:
		return ImGuiKey_CapsLock;
	case Key::SCROLL_LOCK:
		return ImGuiKey_ScrollLock;
	case Key::NUM_LOCK:
		return ImGuiKey_NumLock;
	case Key::PRINT_SCREEN:
		return ImGuiKey_PrintScreen;
	case Key::PAUSE:
		return ImGuiKey_Pause;
	case Key::KP_0:
		return ImGuiKey_Keypad0;
	case Key::KP_1:
		return ImGuiKey_Keypad1;
	case Key::KP_2:
		return ImGuiKey_Keypad2;
	case Key::KP_3:
		return ImGuiKey_Keypad3;
	case Key::KP_4:
		return ImGuiKey_Keypad4;
	case Key::KP_5:
		return ImGuiKey_Keypad5;
	case Key::KP_6:
		return ImGuiKey_Keypad6;
	case Key::KP_7:
		return ImGuiKey_Keypad7;
	case Key::KP_8:
		return ImGuiKey_Keypad8;
	case Key::KP_9:
		return ImGuiKey_Keypad9;
	case Key::KP_DECIMAL:
		return ImGuiKey_KeypadDecimal;
	case Key::KP_DIVIDE:
		return ImGuiKey_KeypadDivide;
	case Key::KP_MULTIPLY:
		return ImGuiKey_KeypadMultiply;
	case Key::KP_SUBTRACT:
		return ImGuiKey_KeypadSubtract;
	case Key::KP_ADD:
		return ImGuiKey_KeypadAdd;
	case Key::KP_ENTER:
		return ImGuiKey_KeypadEnter;
	case Key::KP_EQUAL:
		return ImGuiKey_KeypadEqual;
	case Key::LEFT_SHIFT:
		return ImGuiKey_LeftShift;
	case Key::LEFT_CONTROL:
		return ImGuiKey_LeftCtrl;
	case Key::LEFT_ALT:
		return ImGuiKey_LeftAlt;
	case Key::LEFT_SUPER:
		return ImGuiKey_LeftSuper;
	case Key::RIGHT_SHIFT:
		return ImGuiKey_RightShift;
	case Key::RIGHT_CONTROL:
		return ImGuiKey_RightCtrl;
	case Key::RIGHT_ALT:
		return ImGuiKey_RightAlt;
	case Key::RIGHT_SUPER:
		return ImGuiKey_RightSuper;
	case Key::MENU:
		return ImGuiKey_Menu;
	case Key::NUM_0:
		return ImGuiKey_0;
	case Key::NUM_1:
		return ImGuiKey_1;
	case Key::NUM_2:
		return ImGuiKey_2;
	case Key::NUM_3:
		return ImGuiKey_3;
	case Key::NUM_4:
		return ImGuiKey_4;
	case Key::NUM_5:
		return ImGuiKey_5;
	case Key::NUM_6:
		return ImGuiKey_6;
	case Key::NUM_7:
		return ImGuiKey_7;
	case Key::NUM_8:
		return ImGuiKey_8;
	case Key::NUM_9:
		return ImGuiKey_9;
	case Key::A:
		return ImGuiKey_A;
	case Key::B:
		return ImGuiKey_B;
	case Key::C:
		return ImGuiKey_C;
	case Key::D:
		return ImGuiKey_D;
	case Key::E:
		return ImGuiKey_E;
	case Key::F:
		return ImGuiKey_F;
	case Key::G:
		return ImGuiKey_G;
	case Key::H:
		return ImGuiKey_H;
	case Key::I:
		return ImGuiKey_I;
	case Key::J:
		return ImGuiKey_J;
	case Key::K:
		return ImGuiKey_K;
	case Key::L:
		return ImGuiKey_L;
	case Key::M:
		return ImGuiKey_M;
	case Key::N:
		return ImGuiKey_N;
	case Key::O:
		return ImGuiKey_O;
	case Key::P:
		return ImGuiKey_P;
	case Key::Q:
		return ImGuiKey_Q;
	case Key::R:
		return ImGuiKey_R;
	case Key::S:
		return ImGuiKey_S;
	case Key::T:
		return ImGuiKey_T;
	case Key::U:
		return ImGuiKey_U;
	case Key::V:
		return ImGuiKey_V;
	case Key::W:
		return ImGuiKey_W;
	case Key::X:
		return ImGuiKey_X;
	case Key::Y:
		return ImGuiKey_Y;
	case Key::Z:
		return ImGuiKey_Z;
	case Key::F1:
		return ImGuiKey_F1;
	case Key::F2:
		return ImGuiKey_F2;
	case Key::F3:
		return ImGuiKey_F3;
	case Key::F4:
		return ImGuiKey_F4;
	case Key::F5:
		return ImGuiKey_F5;
	case Key::F6:
		return ImGuiKey_F6;
	case Key::F7:
		return ImGuiKey_F7;
	case Key::F8:
		return ImGuiKey_F8;
	case Key::F9:
		return ImGuiKey_F9;
	case Key::F10:
		return ImGuiKey_F10;
	case Key::F11:
		return ImGuiKey_F11;
	case Key::F12:
		return ImGuiKey_F12;
	default:
		return ImGuiKey_None;
	}
}
}  // namespace FearEngine
