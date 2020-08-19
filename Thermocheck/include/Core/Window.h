#ifndef WINDOW_H
#define WINDOW_H

#include <string>

struct GLFWwindow;

struct WindowProperties {
	std::string title = "Thermocheck App";
	uint32_t width = 1280, height = 720;
};

class Window {
public:
	
	Window();
	Window(const WindowProperties& properties);
	~Window();

	void update() const;
	
	uint32_t getWidth() const { return _properties.width; }
	uint32_t getHeight() const { return _properties.height; }
	
	GLFWwindow* getGlfwWindow() const { return _glfwWindow; }
	
private:

	GLFWwindow* _glfwWindow;
	WindowProperties _properties;
};


#endif