#include "tcpch.h"
#include "GUI/ImGuiLayer.h"

#include "Core/Application.h"

#define IMGUI_IMPL_LOADER_GLAD
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

void ImGuiLayer::init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
	io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	auto* const window = Application::getWindow()->getGlfwWindow();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
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


void ImGuiLayer::drawImGui() {
	static bool demoWindow = true;
	
	if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("Exit")) Application::shutdown();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	
	ImGui::ShowDemoWindow(&demoWindow);
	
}
