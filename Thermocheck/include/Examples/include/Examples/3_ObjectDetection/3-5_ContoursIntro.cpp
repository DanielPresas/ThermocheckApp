#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat img = cv::imread("assets/detect_blob.png", cv::IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::Mat gray; cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
	cv::Mat thresh; cv::adaptiveThreshold(gray, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 115, 1);
	cv::imshow("Binary", thresh);

	std::vector<cv::Mat> contours;
	cv::Mat hierarchy; cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat img2 = img.clone();

	cv::drawContours(img2, contours, -1, { 255, 0, 255 }, 4);
	cv::imshow("Contours", img2);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}