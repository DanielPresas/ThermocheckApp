#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::Mat img = cv::imread("assets/detect_blob.png", cv::IMREAD_COLOR);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::Mat gray; cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
	cv::Mat thresh; cv::adaptiveThreshold(gray, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 115, 1);
	cv::imshow("Binary", thresh);

	std::vector<cv::Mat> contours; cv::Mat hierarchy;
	cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat objects = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
	uint32_t i = 0;
	for(const auto& contour : contours) {
		cv::drawContours(objects, std::array<cv::Mat, 1>{ contour }, -1, { 255, 0, 255 }, cv::FILLED);

		double area = cv::contourArea(contour);
		double perimeter = cv::arcLength(contour, true);

		cv::Moments moment = cv::moments(contour);
		int centroidX = static_cast<int>(moment.m10 / moment.m00);
		int centroidY = static_cast<int>(moment.m01 / moment.m00);
		cv::circle(objects, { centroidX, centroidY }, 4, { 0, 0, 255 }, cv::FILLED);

		// https://fmt.dev/latest/syntax.html to read up on what the heck this all means lol
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