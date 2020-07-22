#include "tcpch.h"
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	Logger::init();

	auto cap = cv::VideoCapture(0);
	const cv::Scalar color = { 0, 255, 0 };
	const int lineWidth = 3;
	const int radius = 100;
	cv::Vec2i point = { 0, 0 };


	cv::namedWindow("Frame");
	cv::setMouseCallback("Frame", [](const int e, const int x, const int y, const int flags, void* userData) {
		if(e == cv::EVENT_LBUTTONDOWN) {
			LOG_INFO("Left button pressed: ({0}, {1})", x, y);
			*static_cast<cv::Vec2i*>(userData) = { x, y };
		}

		if(e == cv::EVENT_MOUSEMOVE) {
			if(flags == cv::EVENT_FLAG_LBUTTON) {
				LOG_INFO("Mouse moved while pressed: ({0}, {1})", x, y);
				*static_cast<cv::Vec2i*>(userData) = { x, y };
			}
		}

	}, &point);

	while(true) {

		cv::Mat frame;
		cap.read(frame);

		cv::resize(frame, frame, { 0, 0 }, 0.5f, 0.5f);
		cv::circle(frame, point, radius, color, lineWidth);
		cv::imshow("Frame", frame);

		const int ch = cv::waitKey(1);
		if(ch == 27 || !static_cast<bool>(cv::getWindowProperty("Frame", cv::WND_PROP_VISIBLE))) {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	Logger::shutdown();
	return 0;
}