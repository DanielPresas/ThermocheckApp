#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::Mat color = imread("assets/starry_night.png", cv::IMREAD_COLOR);
	cv::imshow("Image", color);
	cv::moveWindow("Image", 0, 0);

	auto shape = cv::Vec3i(color.rows, color.cols, color.channels());
	LOG_TRACE("Shape: {}", shape);

	cv::Mat bgrChannels[3];
	cv::split(color, bgrChannels);

	cv::Mat rgbSplit = cv::Mat(color.rows * 3, color.cols, CV_8UC3);
	cv::Mat blueMerge[3] = { bgrChannels[0], bgrChannels[0], bgrChannels[0] };
	cv::Mat greenMerge[3] = { bgrChannels[1], bgrChannels[1], bgrChannels[1] };
	cv::Mat redMerge[3] = { bgrChannels[2], bgrChannels[2], bgrChannels[2] };

	cv::Mat b, g, r;
	cv::merge(blueMerge, 3, b);
	cv::merge(greenMerge, 3, g);
	cv::merge(redMerge, 3, r);

	cv::Mat last[3] = { b, g, r };
	cv::hconcat(last, 3, rgbSplit);

	cv::imshow("Channels", rgbSplit);
	cv::moveWindow("Channels", 0, color.rows);

	cv::Mat hsv, hsvChannels[3], hsvSplit;

	cv::cvtColor(color, hsv, cv::COLOR_BGR2HSV);
	cv::split(hsv, hsvChannels);
	cv::hconcat(hsvChannels, 3, hsvSplit);

	cv::imshow("Split HSV", hsvSplit);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}