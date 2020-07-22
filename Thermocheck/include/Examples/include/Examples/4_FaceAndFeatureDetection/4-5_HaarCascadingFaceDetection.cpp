#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	cv::UMat img; cv::imread("assets/faces.jpeg", cv::IMREAD_COLOR).copyTo(img);
	LOG_ASSERT(!img.empty(), "Failed to load image!");

	cv::UMat gray; cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	auto faceCascade = cv::CascadeClassifier("assets/haarcascade_frontalface_default.xml");
	LOG_ASSERT(!faceCascade.empty(), "Failed to load cascade classifier!");
	std::vector<cv::Rect> faces;

	{
		Timer timer("Face cascade", true);
		faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, { 40, 40 });
	}
	LOG_TRACE("Number of faces detected: {}", faces.size());

	for(const auto& rect : faces) {
		cv::rectangle(img, { rect.x, rect.y }, { rect.x + rect.width, rect.y + rect.height }, { 0, 255, 0 }, 4);
	}

	cv::resize(img, img, { 0,0 }, 0.30, 0.30);
	cv::imshow("Image", img);

	const int ch = cv::waitKey(0);
	if(ch == 27) {
		cv::destroyAllWindows();
	}

	Logger::shutdown();
	return 0;
}