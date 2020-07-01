#include "vdpch.h"

#include "Core/Application.h"

extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main(int argc, char* argv[]) {
#if _DEBUG || _RELEASE
	Logger::init();
#endif
	Application* app = new Application(1280, 720, "Thermocheck App");
	app->run();
	delete app;
	
	return 0;
}
