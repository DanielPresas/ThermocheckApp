#include "tcpch.h"
#include "Core/Window.h"

#include "Core/Application.h"
#include "GUI/GraphicsContext.h"

#include <glfw/glfw3.h>

static bool glfwInitialized = false;

Window::Window() :
	Window(WindowProperties()) {}

Window::Window(const WindowProperties& properties) :
	_properties(properties)
{
	Logger::info("Creating window \"{}\" ({} x {})", properties.title, properties.width, properties.height);

	if(!glfwInitialized) {
		Logger::logAssert(glfwInit(), "Could not initialize GLFW!");
		glfwSetErrorCallback([](const int errorCode, const char* log) {
			Logger::critical("GLFW Error! ({})\n{}", errorCode, log);
		});
		glfwInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_glfwWindow = glfwCreateWindow(static_cast<int>(_properties.width), static_cast<int>(_properties.height), _properties.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(_glfwWindow);
	glfwSetWindowUserPointer(_glfwWindow, &_properties);

	glfwSwapInterval(0);

	GraphicsContext::init();
}


Window::~Window() {
	glfwDestroyWindow(_glfwWindow);
}

void Window::update() const {
	if(glfwWindowShouldClose(_glfwWindow)) Application::shutdown();
	glfwPollEvents();
	glfwSwapBuffers(_glfwWindow);
}