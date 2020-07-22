#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	const cv::Mat templ = cv::imread("assets/template.jpg", cv::IMREAD_GRAYSCALE);
	const cv::Mat frame = cv::imread("assets/players.jpg", cv::IMREAD_GRAYSCALE);

	cv::imshow("Frame", frame);
	cv::imshow("Template", templ);

	cv::Mat result; cv::matchTemplate(frame, templ, result, cv::TM_CCOEFF_NORMED);

	double minVal, maxVal;
	cv::Point minLoc, maxLoc;

	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	LOG_TRACE("Max value: {} at ({}, {})", maxVal, maxLoc.x, maxLoc.y);
	LOG_TRACE("Min value: {} at ({}, {})", minVal, minLoc.x, minLoc.y);

	cv::circle(result, minLoc, 15, 100 / 255.0f, 2);
	cv::circle(result, maxLoc, 15, 255 / 255.0f, 2);

	cv::imshow("Matching", result);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}