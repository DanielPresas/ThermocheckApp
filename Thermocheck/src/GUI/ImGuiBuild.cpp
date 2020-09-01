#include "tcpch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#if PLATFORM_LINUX
#	define IMGUI_IMPL_OPENGL_ES2
#endif

#include <imgui/examples/imgui_impl_opengl3.cpp>
#include <imgui/examples/imgui_impl_glfw.cpp>
