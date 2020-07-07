#include "vdpch.h"

//#include "Core/Application.h"
//
//extern "C" {
//	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
//	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
//}
//
//int main(int argc, char* argv[]) {
//#if _DEBUG || _RELEASE
//	Logger::init();
//#endif
//	Application* app = new Application(1280, 720, "Thermocheck App");
//	app->run();
//	delete app;
//	
//	return 0;
//}


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


int main(int argc, char** argv) {
	if(argc != 2) {
		std::cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << std::endl;
		return -1;
	}
	cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR); // Read the file
	if(image.empty()) // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.
	cv::waitKey(0); // Wait for a keystroke in the window
	return 0;
}