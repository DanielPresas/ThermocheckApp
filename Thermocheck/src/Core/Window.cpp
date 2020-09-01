#include "tcpch.h"
#include "Core/Window.h"


#include "Core/Application.h"
#include "GUI/GraphicsContext.h"

#include <glfw/glfw3.h>
#include <stb/stb_image.h>

static bool glfwInitialized = false;

Window::Window() :
	Window(WindowProperties()) {}

Window::Window(const WindowProperties& properties) :
	_properties(properties)
{
	
#if TC_DEBUG
	TC_LOG_INFO("Creating window \"{}\" ({} x {})", properties.title, properties.width, properties.height);
#endif
	
	if(!glfwInitialized) {
		TC_ASSERT(glfwInit(), "Could not initialize GLFW!");
		glfwSetErrorCallback([](const int errorCode, const char* log) {
			TC_LOG_CRITICAL("GLFW Error! ({})", errorCode);
			TC_LOG_ERROR("{}", log);
		});
		glfwInitialized = true;
	}

#if PLATFORM_WINDOWS
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#endif

#if TC_DEBUG

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	
#endif

	
	_glfwWindow = glfwCreateWindow(static_cast<int>(_properties.width), static_cast<int>(_properties.height), _properties.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(_glfwWindow);
	glfwSetWindowUserPointer(_glfwWindow, &_properties);

	GLFWimage icon;
	icon.pixels = stbi_load("assets/branding/EdiomLogo.png", &icon.width, &icon.height, nullptr, 0);
	glfwSetWindowIcon(_glfwWindow, 1, &icon);
	stbi_image_free(icon.pixels);
	
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
