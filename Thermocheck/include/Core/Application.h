#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/CVInterface.h"
#include "Core/Window.h"

class Application {
public:

	Application();
	Application(std::string&& title, uint32_t width, uint32_t height);
	virtual ~Application();

	Application(const Application&) = delete;

	Application& operator=(const Application&) = delete;

	void run() const;
	// void onEvent();

	static Window* getWindow() { return _instance->_window; }
	
	static void shutdown() { _instance->_isRunning = false; }
	
private:
	
	bool _isRunning = true;
	bool _isMinimized = false;
	
	Window* _window;
	
	
	static Application* _instance;
};

#endif