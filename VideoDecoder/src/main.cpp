#include "vdpch.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#define CH2_5 1

int main(int argc, char** argv) {
	Logger::init();
	using namespace cv;

#if CH2_3
	Mat black = Mat(150, 200, CV_8UC1, Scalar::all(0));
	cv::imshow("Black", black);
	LOG_TRACE("{}", black.at<uchar>(Point(0, 0)));

	Mat ones = Mat(150, 200, CV_8UC3, Scalar::all(1));
	cv::imshow("Ones", ones);
	LOG_TRACE("{}", ones.at<Vec3b>(Point(0, 0)));

	Mat white = Mat(150, 200, CV_16UC3, Scalar::all(std::pow(2, 16) - 1));
	cv::imshow("White", white);
	LOG_TRACE("{}", white.at<Vec3w>(Point(0, 0)));

	Mat color = ones.clone();
	color = Scalar(255, 0, 0, 0);
	imshow("Blue", color);
	LOG_TRACE("{}", color.at<Vec3b>(Point(0, 0)));
	
	cv::waitKey(0);
	cv::destroyAllWindows();
#endif

#if CH2_4

	Mat color = imread("starry_night.png", IMREAD_COLOR);
	cv::imshow("Image", color);
	cv::moveWindow("Image", 0, 0);

	auto shape = Vec3i(color.rows, color.cols, color.channels());
	LOG_TRACE("Shape: {}", shape);

	Mat bgrChannels[3];
	cv::split(color, bgrChannels);

	Mat rgbSplit = Mat(color.rows * 3, color.cols, CV_8UC3);
	Mat blueMerge[3] = { bgrChannels[0], bgrChannels[0], bgrChannels[0] };
	Mat greenMerge[3] = { bgrChannels[1], bgrChannels[1], bgrChannels[1] };
	Mat redMerge[3] = { bgrChannels[2], bgrChannels[2], bgrChannels[2] };
	
	Mat b, g, r;
	cv::merge(blueMerge, 3, b);
	cv::merge(greenMerge, 3, g);
	cv::merge(redMerge, 3, r);

	Mat last[3] = { b, g, r };
	cv::hconcat(last, 3, rgbSplit);	

	cv::imshow("Channels", rgbSplit);
	cv::moveWindow("Channels", 0, color.rows);

	Mat hsv, hsvChannels[3], hsvSplit;

	cv::cvtColor(color, hsv, COLOR_BGR2HSV);
	cv::split(hsv, hsvChannels);
	cv::hconcat(hsvChannels, 3, hsvSplit);

	cv::imshow("Split HSV", hsvSplit);	
	
	waitKey(0);
	destroyAllWindows();
	
#endif

	
	
	return 0;
}