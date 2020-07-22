#include "tcpch.h"
#include <opencv2/opencv.hpp>

#define EXAMPLE 0

#if EXAMPLE

#include <Examples/3_ObjectDetection/3-3_AdaptiveThresholding.cpp>

#else

int main(int argc, char** argv) {

	Logger::init();
	using namespace cv;

	auto cam = VideoCapture(1);

	cv::namedWindow("Capture");

	while(true) {
		UMat frame, frameCopy;
		LOG_ASSERT(cam.read(frame), "Failed to read frame from capture device!");
		cv::imshow("Capture", frame);
		frameCopy = frame.clone();

		UMat hsv; cv::cvtColor(frame, hsv, COLOR_BGR2HSV);
		std::array<Mat, 3> hsvChannels; cv::split(hsv, hsvChannels);

		UMat minSat, maxHue;
		UMat finalMat;
		cv::threshold(hsvChannels[0], maxHue, 15, 255, THRESH_BINARY_INV);
		cv::threshold(hsvChannels[1], minSat, 40, 255, THRESH_BINARY);
		cv::bitwise_and(minSat, maxHue, finalMat);
		cv::imshow("HSV Split", finalMat);

		std::vector<Mat> contours; cv::findContours(finalMat, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
		
		for(uint32_t i = 0; i < contours.size(); ++i) {
			if(cv::contourArea(contours.at(i)) < 1000.0) continue;
			cv::drawContours(frameCopy, contours, i, { (double)Random::UInt(0, 255), (double)Random::UInt(0, 255), (double)Random::UInt(0, 255) }, 2);
		}
		cv::imshow("Contour Skin Detect", frameCopy);
		
		const auto ch = cv::waitKey(1);
		if(ch == 27 || !static_cast<bool>(cv::getWindowProperty("Capture", WND_PROP_VISIBLE))) {
			break;
		}
	}
	
	cam.release();
	cv::destroyAllWindows();
	
	Logger::shutdown();
	
	return 0;
}

#endif