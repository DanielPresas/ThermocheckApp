#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat img = cv::imread("assets/sudoku.png", cv::IMREAD_GRAYSCALE);
	cv::imshow("Original", img);

	cv::Mat threshBasic;
	{
		Timer timer("Threshold basic", true);
		cv::threshold(img, threshBasic, 70, 255, cv::THRESH_BINARY);
	}
	cv::imshow("Basic Binary", threshBasic);

	cv::Mat adaptiveThresh;
	{
		Timer timer("Adaptive threshold", true);
		cv::adaptiveThreshold(img, adaptiveThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 115, 1);
	}
	cv::imshow("Adaptive Binary", adaptiveThresh);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}