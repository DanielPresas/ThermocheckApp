#include "vdpch.h"
#include "Core/Application.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

bool Application::glfwInitialized = false;

static void glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	// ignore non-significant error/warning codes
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	char buffer[9] = { '\0' };
	sprintf(buffer, "%.8x", id);

	std::string log = "OpenGL(0x" + std::string(buffer) + "): ";

	switch(type) {
		case GL_DEBUG_TYPE_ERROR:					log += "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		log += "DEPRECATED BEHAVIOUR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		log += "UNDEFINED BEHAVIOUR"; break;
		case GL_DEBUG_TYPE_PORTABILITY:				log += "PORTABILITY ISSUE"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:				log += "PERFORMANCE ISSUE"; break;
		case GL_DEBUG_TYPE_MARKER:					log += "TYPE MARKER"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:				log += "PUSH GROUP"; break;
		case GL_DEBUG_TYPE_POP_GROUP:				log += "POP GROUP"; break;
		case GL_DEBUG_TYPE_OTHER:					log += "OTHER"; break;
		default: break;
	}

	log += "\n\tSOURCE: ";
	switch(source) {
		case GL_DEBUG_SOURCE_API:					log += "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:			log += "Window system"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:		log += "Shader compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:			log += "Third party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:			log += "Application"; break;
		case GL_DEBUG_SOURCE_OTHER:					log += "Other"; break;
		default: break;
	}

	log += " \n\tSEVERITY: ";
	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:				log += "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:				log += "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW:					log += "LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:		log += "NOTIFICATION"; break;
		default: break;
	}

	log += "\n\n" + std::string(message);

	if(type == GL_DEBUG_TYPE_ERROR) {
		LOG_ASSERT(type != GL_DEBUG_TYPE_ERROR, "{0}", log);
	}
	else if(severity == GL_DEBUG_SEVERITY_HIGH) {
		LOG_ERROR("{}", log);
	}
	else if(severity == GL_DEBUG_SEVERITY_MEDIUM) {
		LOG_WARNING("{}", log);
	}
	else if(severity == GL_DEBUG_SEVERITY_LOW) {
		LOG_INFO("{}", log);
	}
	else {
		LOG_TRACE("{0}", log);
	}
}


Application::Application(const uint32_t width, const uint32_t height, const std::string& title) {
	LOG_INFO("Creating window \"{0}\" ({1} x {2})", title, width, height);

	if(!glfwInitialized) {
		const int glfwStatus = glfwInit();
		LOG_ASSERT(glfwStatus, "Could not initialize GLFW!");
		glfwSetErrorCallback([](const int errorCode, const char* log) {
			LOG_CRITICAL("GLFW Error! ({0})\n{1}", errorCode, log);
		});
		glfwInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(0);

	const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	LOG_ASSERT(gladStatus, "Could not initialize GLAD!");

#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glErrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

	LOG_INFO("OpenGL version {0}\n\tGLSL version {1}\n\tUsing {2} {3}", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));


	
}

Application::~Application() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Application::run() {
	while (!glfwWindowShouldClose(_window)) {
		glClearColor(0.8f, 0.2f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}
