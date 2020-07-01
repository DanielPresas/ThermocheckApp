#ifndef APPLICATION_H
#define APPLICATION_H

struct GLFWwindow;

class Application {
public:
	
	Application(uint32_t width, uint32_t height, const std::string& title);
	virtual ~Application();

	void run();

	uint32_t getWidth() const { return _width; }
	uint32_t getHeight() const { return _height; }
	

protected:

	GLFWwindow* _window = nullptr;
	uint32_t _width = 0, _height = 0;
	
	bool _isRunning = true;
	bool _isMinimized = false;

	static bool glfwInitialized;
	
};

#endif