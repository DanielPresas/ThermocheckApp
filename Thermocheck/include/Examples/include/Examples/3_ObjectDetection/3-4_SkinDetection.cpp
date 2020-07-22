#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::Mat img = cv::imread("assets/faces.jpeg", cv::IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::resize(img, img, { 0, 0 }, 0.15, 0.15);

	cv::Mat hsv;
	cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

	cv::Mat hsvChannels[3], hsvSplit;
	cv::split(hsv, hsvChannels);
	cv::hconcat(hsvChannels, 3, hsvSplit);
	cv::imshow("Split HSV", hsvSplit);

	cv::Mat minSaturation;
	cv::threshold(hsvChannels[1], minSaturation, 40, 255, cv::THRESH_BINARY);
	cv::imshow("Saturation Filter", minSaturation);

	cv::Mat maxHue;
	cv::threshold(hsvChannels[0], maxHue, 15, 255, cv::THRESH_BINARY_INV);
	cv::imshow("Hue Filter", maxHue);

	cv::Mat finalMat;
	cv::bitwise_and(minSaturation, maxHue, finalMat);
	cv::imshow("Final", finalMat);
	cv::imshow("Original", img);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}