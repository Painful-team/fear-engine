#define UNICODE

#include <iostream>
#include <windows.h>

#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


constexpr int Width = 600;
constexpr int Heigth = 600;

void Resize(GLFWwindow* window, int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

DWORD WINAPI worker(LPVOID lpParameter)
{
	while (true)
	{
		std::cout << "Working\n";
		Sleep(1);
	}
}

int main()
{
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	GLFWwindow* window = glfwCreateWindow(Width, Heigth, "Controls", NULL, NULL);
	if (!window)
	{
		std::cerr << "Windows doesn't created.";
		glfwTerminate();
		return -1;
	}

	//glDebugMessageCallback(MessageCallback, nullptr);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Resize);

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cerr << "Couldn't found glad.";
		return -2;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	bool thread = false;
	bool suspended = false;
	bool boostThread = false;

	bool proccess = false;

	bool boost = false;
	int currentPriority = 0;
	char buf[200] = "C:\\\\WINDOWS\\\\SYSTEM32\\\\NOTEPAD.EXE temp.txt";

	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(startInfo);

	HANDLE handler;
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);

		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::SetNextWindowPos({ 0, 0 });
			ImGui::SetWindowSize({ Width / 2, Heigth / 2 });
			ImGui::Begin("Process", nullptr, ImGuiWindowFlags_NoCollapse);
			ImGui::Text("Status:");
			ImGui::SameLine(Width / 2 + 120);

			if (ImGui::SmallButton("Exit"))
			{
				ExitProcess(0);
			}

			if (!proccess)
			{
				ImGui::SameLine(60);
				ImGui::Text("Terminated");
				ImGui::InputText("CommandLine 200 max", buf, 200);

				if (ImGui::Button("Run"))
				{
					WCHAR proc_name[256];
					mbstowcs(&proc_name[0], buf, strlen(buf) + 1);
					CreateProcess(NULL, proc_name, NULL, NULL, FALSE, HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &processInfo);

					int boosted;
					GetProcessPriorityBoost(processInfo.hProcess, &boosted);
					boost = boosted;
					proccess = true;
				}

			}

			DWORD isRunning;
			if (proccess)
			{
				GetExitCodeProcess(processInfo.hProcess, &isRunning);
				if (isRunning == STILL_ACTIVE)
				{
					ImGui::SameLine(60);
					ImGui::Text("Running");

					if (ImGui::Checkbox("PriorityBoost", &boost))
					{
						SetProcessPriorityBoost(processInfo.hProcess, boost);
					}

					int currentPriority = 0;
					switch (GetPriorityClass(processInfo.hProcess))
					{
					case ABOVE_NORMAL_PRIORITY_CLASS:
						ImGui::Text("Priority: above normal and lower that high");
						currentPriority = 0;
						break;
					case BELOW_NORMAL_PRIORITY_CLASS:
						ImGui::Text("Priority: above Idle and lower that normal");
						currentPriority = 1;
						break;
					case HIGH_PRIORITY_CLASS:
						ImGui::Text("Priority: High");
						currentPriority = 2;
						break;
					case IDLE_PRIORITY_CLASS:
						ImGui::Text("Priority: Idle");
						currentPriority = 3;
						break;
					case NORMAL_PRIORITY_CLASS:
						ImGui::Text("Priority: Normal");
						currentPriority = 4;
						break;
					case REALTIME_PRIORITY_CLASS:
						ImGui::Text("Priority: Highest");
						break;
					};

					const char* comboText[]
					{
						"Above_Normal",
						"Below_Normal",
						"High",
						"Idle",
						"Normal"
					};

					ImGui::SetNextItemWidth(130);
					if (ImGui::Combo("SetPriority", &currentPriority, comboText, IM_ARRAYSIZE(comboText)))
					{
						switch (currentPriority)
						{
						case 0:
							SetPriorityClass(processInfo.hProcess, ABOVE_NORMAL_PRIORITY_CLASS);
							break;
						case 1:
							SetPriorityClass(processInfo.hProcess, BELOW_NORMAL_PRIORITY_CLASS);
							break;
						case 2:
							SetPriorityClass(processInfo.hProcess, HIGH_PRIORITY_CLASS);
							break;
						case 3:
							SetPriorityClass(processInfo.hProcess, IDLE_PRIORITY_CLASS);
							break;
						case 4:
							SetPriorityClass(processInfo.hProcess, NORMAL_PRIORITY_CLASS);
							break;
						};
					}

					if (ImGui::Button("Terminate"))
					{
						TerminateProcess(processInfo.hProcess, 0);
						proccess = false;
					}
				}
				else
				{
					proccess = false;
				}
			}

			ImGui::End();

			ImGui::SetNextWindowPos({ 0, 300 });
			ImGui::SetWindowSize({ Width / 2, Heigth / 2 });
			ImGui::Begin("Thread");

			ImGui::Text("Status:");
			ImGui::SameLine(Width / 2 + 120);

			if (ImGui::SmallButton("Exit"))
			{
				ExitProcess(0);
			}

			if (!thread)
			{
				ImGui::SameLine(60);
				ImGui::Text("Undefined");

				if (ImGui::Button("Create"))
				{
					handler = CreateThread(NULL, 0, worker, 0, 0, NULL);

					int boosted = GetThreadPriority(handler);
					boostThread = boosted;
					thread = true;
				}
			}

			if (thread)
			{

				if (!suspended)
				{
					ImGui::SameLine(60);
					ImGui::Text("Running");

					if (ImGui::Checkbox("PriorityBoost", &boostThread))
					{
						SetThreadPriorityBoost(handler, boostThread);
					}

					int currentPriority = 0;
					switch (GetThreadPriority(handler))
					{
					case THREAD_PRIORITY_ABOVE_NORMAL:
						ImGui::Text("Priority: above normal");
						currentPriority = 0;
						break;
					case THREAD_PRIORITY_BELOW_NORMAL:
						ImGui::Text("Priority: below normal");
						currentPriority = 1;
						break;
					case THREAD_PRIORITY_HIGHEST:
						ImGui::Text("Priority: Highest");
						currentPriority = 2;
						break;
					case THREAD_PRIORITY_IDLE:
						ImGui::Text("Priority: Idle");
						currentPriority = 3;
						break;
					case THREAD_PRIORITY_LOWEST:
						ImGui::Text("Priority: lowest");
						currentPriority = 4;
						break;
					case THREAD_PRIORITY_NORMAL:
						ImGui::Text("Priority: normal");
						currentPriority = 5;
						break;
					case THREAD_PRIORITY_TIME_CRITICAL:
						ImGui::Text("Priority: Critical");
						currentPriority = 6;
						break;
				};

					const char* comboText[]
					{
						"Above_Normal",
						"Below_Normal",
						"Highest",
						"Idle",
						"lowest",
						"Normal",
						"Critical"
					};

					ImGui::SetNextItemWidth(130);
					if (ImGui::Combo("SetPriority", &currentPriority, comboText, IM_ARRAYSIZE(comboText)))
					{
						switch (currentPriority)
						{
						case 0:
							SetThreadPriority(handler, THREAD_PRIORITY_ABOVE_NORMAL);
							break;
						case 1:
							SetThreadPriority(handler, THREAD_PRIORITY_BELOW_NORMAL);
							break;
						case 2:
							SetThreadPriority(handler, THREAD_PRIORITY_HIGHEST);
							break;
						case 3:
							SetThreadPriority(handler, THREAD_PRIORITY_IDLE);
							break;
						case 4:
							SetThreadPriority(handler, THREAD_PRIORITY_LOWEST);
							break;
						case 5:
							SetThreadPriority(handler, THREAD_PRIORITY_NORMAL);
							break;
						case 6:
							SetThreadPriority(handler, THREAD_PRIORITY_TIME_CRITICAL);
							break;
						};
					}

					if (ImGui::Button("Suspend"))
					{
						SuspendThread(handler);
						std::cout << "Suspended";
						suspended = true;
					}
				}
			else
			{
				ImGui::SameLine(60);
				ImGui::Text("Suspended");
				if (ImGui::Button("Resume"))
				{
					ResumeThread(handler);
					suspended = false;
				}
			}
		}

		ImGui::End();
	}

		ImGui::Render();

		glClearColor(0.0f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	if (proccess)
	{
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}

	if (thread)
	{
		CloseHandle(processInfo.hThread);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
