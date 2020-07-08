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


int main(int argc, char** argv) {
	Logger::init();
	
	cv::samples::addSamplesDataSearchPath("C:/Repos/Brilliant Catalyst/Camera Video Decoder/Externals/opencv/opencv/samples/data");
	std::string image_path = cv::samples::findFile("starry_night.jpg", true, true);
	cv::Mat img = imread(image_path, cv::IMREAD_GRAYSCALE);
	LOG_ASSERT(img.empty(), "Could not read the image: {}", image_path);
	cv::imshow("Thermocheck App", img);
	int k = cv::waitKey(0); // Wait for a keystroke in the window
	if(k == 's') {
		cv::imwrite("starry_night.png", img);
	}
	
	return 0;
}