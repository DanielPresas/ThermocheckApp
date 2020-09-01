#include "tcpch.h"

#include "Core/Application.h"

#define EXAMPLE 0

#if EXAMPLE

//#include <Examples/4_FaceAndFeatureDetection/4-5_HaarCascadingFaceDetection.cpp>

#else

#if PLATFORM_WINDOWS
#	define GPU_VISIBILITY __declspec(dllexport)
#elif PLATFORM_LINUX
#	define GPU_VISIBILITY __attribute__((visibility("default")))
#endif

extern "C" {
	GPU_VISIBILITY unsigned long NvOptimusEnablement = 0x01;
	GPU_VISIBILITY int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main(int argc, char** argv) {
	Logger::init();
	
	auto* app = new Application("Thermocheck App", 1280, 720);
	app->run();
	delete app;
	
	Logger::shutdown();
	
	return 0;
}

#endif
