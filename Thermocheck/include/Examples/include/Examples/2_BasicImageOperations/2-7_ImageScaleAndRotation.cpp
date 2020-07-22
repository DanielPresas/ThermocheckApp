#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::Mat image = cv::imread("assets/starry_night.png", cv::IMREAD_COLOR);
	LOG_ASSERT(!image.empty(), "Could not load image!");

	// Scaling
	cv::Mat halfImage, stretchImage, stretchNear;
	cv::resize(image, halfImage, { 0, 0 }, 0.5, 0.5);
	cv::resize(image, stretchImage, { 600, 600 });
	cv::resize(image, stretchNear, { 600, 600 }, 0, 0, cv::INTER_NEAREST);

	cv::imshow("Half", halfImage);
	cv::imshow("Stretch", stretchImage);
	cv::imshow("Stretch Nearest", stretchNear);

	// Rotation
	cv::Mat rotationMat = cv::getRotationMatrix2D({ 0, 0 }, -30, 1);
	cv::Mat rotated;
	cv::warpAffine(image, rotated, rotationMat, rotated.size());

	cv::imshow("Rotated", rotated);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}