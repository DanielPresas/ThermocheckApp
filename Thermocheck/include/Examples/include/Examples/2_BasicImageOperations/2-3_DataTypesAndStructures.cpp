#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::Mat black = cv::Mat(150, 200, CV_8UC1, cv::Scalar::all(0));
	cv::imshow("Black", black);
	LOG_TRACE("{}", black.at<uchar>(cv::Point(0, 0)));

	cv::Mat ones = cv::Mat(150, 200, CV_8UC3, cv::Scalar::all(1));
	cv::imshow("Ones", ones);
	LOG_TRACE("{}", ones.at<cv::Vec3b>(cv::Point(0, 0)));

	cv::Mat white = cv::Mat(150, 200, CV_16UC3, cv::Scalar::all(std::pow(2, 16) - 1));
	cv::imshow("White", white);
	LOG_TRACE("{}", white.at<cv::Vec3w>(cv::Point(0, 0)));

	cv::Mat color = ones.clone();
	color = cv::Scalar(255, 0, 0, 0);
	cv::imshow("Blue", color);
	LOG_TRACE("{}", color.at<cv::Vec3b>(cv::Point(0, 0)));

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}