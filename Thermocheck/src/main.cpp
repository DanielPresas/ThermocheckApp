#include "tcpch.h"

#include "Core/Application.h"

#define EXAMPLE 0

#if EXAMPLE

//#include <Examples/4_FaceAndFeatureDetection/4-5_HaarCascadingFaceDetection.cpp>

#else

extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
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
