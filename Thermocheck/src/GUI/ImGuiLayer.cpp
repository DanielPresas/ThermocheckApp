#include "tcpch.h"
#include "GUI/ImGuiLayer.h"

#include "Core/Application.h"
#include "GUI/ImGuiConsole.h"

#define IMGUI_IMPL_LOADER_GLAD
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

static const char* imguiIniPath = R"(assets/imgui/imgui.ini)";

void ImGuiLayer::init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
	io.IniFilename = imguiIniPath;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	auto* const window = Application::getWindow()->getGlfwWindow();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGuiLayer::shutdown() {
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool dockspaceOpen = true;
	static bool optFullscreenPersistent = true;
	bool optFullscreen = optFullscreenPersistent;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if(optFullscreen) {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if(dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
		windowFlags |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
	ImGui::PopStyleVar();

	if(optFullscreen) {
		ImGui::PopStyleVar(2);
	}

	ImGuiIO& io = ImGui::GetIO();
	if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	
}

void ImGuiLayer::end() {

	ImGui::End();
	
	ImGuiIO& io = ImGui::GetIO();
	const auto* window = Application::getWindow();
	io.DisplaySize = { static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* currentContextBackup = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(currentContextBackup);
	}
}

static const char* aboutBody =
R"(This app is designed to detect temps and tell the user if their temp is flagged as being high.
Questions/concerns, email Ediom at [insert email here]
)";

void ImGuiLayer::drawImGui() {
	static bool aboutWindow = false;
	static bool demoWindow = false;
	static bool consoleWindow = false;
	static bool metricsWindow = false;

	// ----------------------------------------------------------------
	// ----- MENU BAR -------------------------------------------------
	// ----------------------------------------------------------------
	{
		if(ImGui::BeginMenuBar()) {

			// File menu
			if(ImGui::BeginMenu("Application")) {
				if(ImGui::MenuItem("Show Console")) consoleWindow = true;
				if(ImGui::MenuItem("Show Metrics")) metricsWindow = true;
				if(ImGui::MenuItem("Exit")) {
					Application::shutdown();
				}
			
				ImGui::EndMenu();
			}

			// Help/About menu
			if(ImGui::BeginMenu("Help")) {
				if(ImGui::MenuItem("About"))                aboutWindow = true;
				if(ImGui::MenuItem("ImGui Demo Window"))    demoWindow  = true;

				ImGui::EndMenu();
			}
		
			ImGui::EndMenuBar();
		}
	}

	// ----------------------------------------------------------------
	// ----- METRICS WINDOW -------------------------------------------
	// ----------------------------------------------------------------
	{
		if(metricsWindow) {
			ImGui::Begin("Metrics", &metricsWindow);
			{
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui::TextWrapped("OpenGL version: %s", glGetString(GL_VERSION));
				ImGui::TextWrapped("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
				ImGui::TextWrapped("Using %s %s", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
				ImGui::TextWrapped("Framerate: %.3f FPS", io.Framerate);
				ImGui::TextWrapped("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
			}
			ImGui::End();
		}
	}
	
	// ----------------------------------------------------------------
	// ----- ABOUT WINDOW ---------------------------------------------
	// ----------------------------------------------------------------
	{
		if(aboutWindow) {
			
			ImGui::SetNextWindowContentSize({ 400, 300 });
			const ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(centre, ImGuiCond_Always, { 0.5f, 0.5f });

			const ImGuiWindowFlags flags = 
				ImGuiWindowFlags_NoMove           |
				ImGuiWindowFlags_NoCollapse       |
				ImGuiWindowFlags_NoDocking        |
				ImGuiWindowFlags_NoResize         |
				ImGuiWindowFlags_NoSavedSettings  |
				ImGuiWindowFlags_AlwaysAutoResize;
		
			ImGui::Begin("About", nullptr, flags);
			{
				ImGui::Text("Thermocheck App");
				ImGui::NewLine();

				ImGui::TextWrapped(aboutBody);
				ImGui::NewLine();
				
				ImGui::Text("(R) 2020 Ediom Technologies");
				ImGui::NewLine(); ImGui::NewLine();

				// @Incomplete: Center the close button in the window
				if(ImGui::Button("Close")) aboutWindow = false;
			}
			ImGui::End();
		}
	}

	// ----------------------------------------------------------------
	// ----- LOG CONSOLE WINDOW ---------------------------------------
	// ----------------------------------------------------------------
	{
		if(consoleWindow) {
			ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);
			ImGuiConsole::drawImGui(&consoleWindow);
		}
	}
	
	// ----------------------------------------------------------------
	// ----- IMGUI DEMO WINDOW ----------------------------------------
	// ----------------------------------------------------------------
	{
		if(demoWindow) {
			ImGui::ShowDemoWindow(&demoWindow);
		}
	}
}
