#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::UMat img; cv::imread("assets/faces.jpeg", cv::IMREAD_COLOR).copyTo(img);
	LOG_ASSERT(!img.empty(), "Failed to load image!");
	cv::UMat gray; cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	auto eyeCascade = cv::CascadeClassifier("assets/haarcascade_eye.xml");
	LOG_ASSERT(!eyeCascade.empty(), "Failed to load cascade classifier!");

	std::vector<cv::Rect> eyes;
	{
		Timer timer("Eye detection", true);
		eyeCascade.detectMultiScale(gray, eyes, 1.02, 20, 0, { 10, 10 });
	}
	LOG_TRACE("Number of eyes detected: {}", eyes.size());

	for(const auto& eye : eyes) {
		const double xc = (eye.x + eye.x + eye.width) / 2.0;
		const double yc = (eye.y + eye.y + eye.height) / 2.0;
		const double radius = eye.width / 2.0;

		cv::circle(img, { static_cast<int>(xc), static_cast<int>(yc) }, static_cast<int>(radius), { 255, 0, 0 }, 4);
	}

	cv::resize(img, img, { 0,0 }, 0.30, 0.30);
	cv::imshow("Eyes", img);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}