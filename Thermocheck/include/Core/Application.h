#ifndef APPLICATION_H
#define APPLICATION_H

#include "CVInterface.h"
#include "Core/Window.h"

#include <queue>

class Application {

	enum WorkType {
		CV_UPDATE = 0,
		IMGUI_RENDER = 1
	};

	using FutureVoids = std::vector<std::future<void>>;
	using WorkQueue = std::queue<WorkType>;
	using Mutex = std::mutex;
	
public:

	Application();
	Application(std::string&& title, uint32_t width, uint32_t height);
	virtual ~Application();

	Application(const Application&) = delete;

	Application& operator=(const Application&) = delete;

	void run();
	static void shutdown() { _instance->_isRunning = false; }

	static Window* getWindow()   { return _instance->_window; }
	static CVState* getCVState() { return _instance->_cvState; }

	static void pushCvUpdate(CVState* state);
	static void pushImGuiRender();

private:

	
	bool _isRunning = true;
	
	Window* _window;
	CVState* _cvState;

	WorkQueue _commandQueue;
	FutureVoids _futures;
	Mutex _mutex;
	
	static Application* _instance;
};

#endif