#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat image = cv::imread("assets/starry_night.png");
	LOG_ASSERT(!image.empty(), "Could not load image!");
	cv::imshow("Original", image);

	cv::Mat blur;
	cv::GaussianBlur(image, blur, { 5, 55 }, 0);
	cv::imshow("Blur", blur);

	const cv::Mat kernel = cv::Mat::ones({ 5, 5 }, CV_8U);
	cv::Mat dilate, erode;
	cv::dilate(image, dilate, kernel, { -1, -1 }, 1);
	cv::erode(image, erode, kernel, { -1, -1 }, 1);

	cv::imshow("Dilate", dilate);
	cv::imshow("Erode", erode);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}