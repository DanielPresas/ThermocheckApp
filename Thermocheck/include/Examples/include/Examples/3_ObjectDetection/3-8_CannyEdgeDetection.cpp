#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat img = cv::imread("assets/tomatoes.jpg", cv::IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::Mat hsv; cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
	std::array<cv::Mat, 3> hsvChannels; cv::split(hsv, hsvChannels);
	cv::Mat thresh; cv::threshold(hsvChannels.at(0), thresh, 25, 255, cv::THRESH_BINARY_INV);
	cv::imshow("Threshold", thresh);

	cv::Mat edges; cv::Canny(img, edges, 100, 70);
	cv::imshow("Canny edges", edges);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}