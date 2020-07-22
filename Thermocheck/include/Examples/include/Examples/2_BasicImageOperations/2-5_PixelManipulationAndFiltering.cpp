#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat color = cv::imread("assets/starry_night.png", cv::IMREAD_COLOR);
	cv::Mat gray;
	cv::cvtColor(color, gray, cv::COLOR_RGB2GRAY);
	cv::imwrite("outputs/gray.jpg", gray);

	cv::Mat channels[3];
	cv::split(color, channels);

	cv::Mat rgba, rgbaChannels[4] = { channels[0], channels[1], channels[2], channels[1] };
	cv::merge(rgbaChannels, 4, rgba);
	cv::imwrite("outputs/rgba.png", rgba);

	Logger::shutdown();
	return 0;
}