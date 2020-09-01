#include "tcpch.h"
#include "Core/Application.h"

#include "Core/CVInterface.h"
#include "GUI/ImGuiLayer.h"

#include <glad/glad.h>

Application* Application::_instance = nullptr;

Application::Application() :
	Application("Thermocheck App", 100, 100) {}

Application::Application(std::string&& title, const uint32_t width, const uint32_t height) {
	const bool success = _instance == nullptr;
	TC_ASSERT(success, "Application instance already exists!");
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

void Application::run() {

	while(!_commandQueue.empty()) {
		if(!_isRunning) {
			_commandQueue.pop();
			continue;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if(_commandQueue.front() == CV_UPDATE) {
			_futures.push_back(std::async(std::launch::async, CVInterface::update));
		}
		else if(_commandQueue.front() == IMGUI_RENDER) {
			ImGuiLayer::begin();
			{
				ImGuiLayer::drawImGui();
				std::lock_guard<Mutex> lock(_mutex);
				CVInterface::drawImGui();
			}
			ImGuiLayer::end();

			_window->update();
		}

		{
			std::lock_guard<Mutex> lock(_mutex);
			_commandQueue.pop();
		}
	}

	//
	// Call get() on the last async future to ensure the
	// CVInterface::update() thread finishes running before the program ends
	//
	_futures.back().get();
}

void Application::pushCvUpdate(CVState* state) {
	std::lock_guard<Mutex> lock(_instance->_mutex);
	_instance->_cvState = state;
	_instance->_commandQueue.push(CV_UPDATE);
}

void Application::pushImGuiRender() {
	std::lock_guard<Mutex> lock(_instance->_mutex);
	_instance->_commandQueue.push(IMGUI_RENDER);
}
