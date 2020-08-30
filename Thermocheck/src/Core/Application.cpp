#include "tcpch.h"
#include "Core/Application.h"

#include "Core/CVInterface.h"
#include "GUI/ImGuiLayer.h"

#include <glad/glad.h>

Application* Application::_instance = nullptr;

Application::Application() :
	Application("Thermocheck App", 100, 100) {}

Application::Application(std::string&& title, const uint32_t width, const uint32_t height) {
	TC_ASSERT(_instance == nullptr, "Application instance already exists!");
	_instance = this;

	_window = new Window({ std::move(title), width, height });
	
	CVInterface::init();
	ImGuiLayer::init();
}

Application::~Application() {
	ImGuiLayer::shutdown();
	CVInterface::shutdown();
	delete _window;
	_instance = nullptr;
}

void Application::run() const {

	while(_isRunning) {

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		if(!_isMinimized) {

			CVInterface::update();
			
			ImGuiLayer::begin();
			{
				ImGuiLayer::drawImGui();
				CVInterface::drawImGui();
			}
			ImGuiLayer::end();
			
			_window->update();
			
		}
		
	}
	
}
