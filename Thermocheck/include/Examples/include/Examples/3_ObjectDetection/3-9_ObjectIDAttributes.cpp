#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat img = cv::imread("assets/fuzzy.png", cv::IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::imshow("Original", img);

	cv::Mat thresh;
	cv::cvtColor(img, thresh, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(thresh, thresh, { 3, 3 }, 0);
	cv::adaptiveThreshold(thresh, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 205, 1);
	cv::imshow("Binary", thresh);

	std::vector<cv::Mat> contours; cv::findContours(thresh, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	LOG_INFO("Number of contours: {}", contours.size());

	std::vector<cv::Mat> filtered;
	for(const auto& contour : contours) {
		if(cv::contourArea(contour) < 1000.0) continue;
		filtered.push_back(contour);
	}
	LOG_INFO("Number of contours with area >1000: {}", filtered.size());

	cv::Mat objects = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
	uint32_t i = 0;
	for(const auto& c : filtered) {
		cv::Scalar col = { static_cast<double>(Random::UInt(0, 255)), static_cast<double>(Random::UInt(0, 255)), static_cast<double>(Random::UInt(0, 255)) };
		cv::drawContours(objects, std::vector<cv::Mat>{ c }, -1, col, cv::FILLED);

		double area = cv::contourArea(c);
		double perimeter = cv::arcLength(c, true);
		LOG_TRACE("Contour {0:02} [ Area: {1:>05.2F}, Perimeter: {2:>05.2F} ]", ++i, area, perimeter);
	}

	cv::imshow("Contours", objects);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}